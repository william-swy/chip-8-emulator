#include "cpu.h"

#include "memory.h"

arch::CPU::CPU() {
  index_reg = 0;
  pc_reg = pc_start_value;
  sp_reg = 0;

  general_reg.fill(0);
  delay_timer_reg = 0;
  sound_timer_reg = 0;
  stack.fill(0);
  curr_opcode = 0;

  updated_screen = false;

  const std::string seed_str("RNG seed string");
  const std::seed_seq seed(seed_str.begin(), seed_str.end());
  gen = std::mt19937(seed);
  rng = std::uniform_int_distribution<>(0x00, 0xFF);
}

void arch::CPU::fetch(Memory& mem) {
  // Instructions are 2 bytes, hence need to get current program counter and the next address to
  // build opcode. It is also big endian, hence mem[PC] represents the left most value and mem[PC+1]
  // represents the right most value. Will throw a InvalidMemoryAddress exception if PC holds a
  // value that is either at the greatest memory address or greater.

  const auto left_most = static_cast<unsigned>(mem.get_value(pc_reg));
  const auto right_most = static_cast<unsigned>(mem.get_value(pc_reg + 1));
  curr_opcode = static_cast<unsigned short>(left_most << 8 | right_most);
  pc_reg += 2;
}

void arch::CPU::decode_execute(Memory& mem, Graphics& graphics, Keypad& keypad) {
  // Parse out first 4 bits
  updated_screen = false;
  switch (curr_opcode & 0xF000) {
    case 0x0000:
      switch (curr_opcode) {
        case 0x00E0:
          // Clears the screen
          graphics.clear_screen();
          updated_screen = true;
          break;
        case 0x00EE:
          // Set program counter to top of stack. Decrease stack pointer by 1.
          pc_reg = stack[sp_reg];
          sp_reg--;
          break;
        default:
          throw InvalidInstruction();
      }
      break;
    case 0x1000:
      // Of form 1NNN. Jumps to address NNN
      {
        const auto address = static_cast<unsigned short>(curr_opcode & 0x0FFF);
        pc_reg = address;
      }
      break;
    case 0x2000:
      // Of form 2NNN. Increment stack pointer and store current program counter on stack. Stack
      // pointer is then set to address NNN
      {
        const auto address = static_cast<unsigned short>(curr_opcode & 0x0FFF);
        sp_reg++;
        stack[sp_reg] = pc_reg;
        pc_reg = address;
      }
      break;
    case 0x3000:
      // Of form 3XNN. Skips the next instruction if value of register X equals NN
      {
        // Mask to get the register id and bitshift right 8 to remove the two trailing zeros.
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto value = static_cast<unsigned char>(curr_opcode & 0x00FF);
        if (general_reg[reg_id] == value) {
          pc_reg += 2;
        }
      }
      break;
    case 0x4000:
      // Of form 4XNN. Skips the next instruction if the value of register X does not equal NN
      {
        // Mask to get the register id and bitshift right 8 to remove the two trailing zeros.
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto value = static_cast<unsigned char>(curr_opcode & 0x00FF);
        if (general_reg[reg_id] != value) {
          pc_reg += 2;
        }
      }
      break;
    case 0x5000:
      // Must be of form 5XY0. Skips the next instruction of the value of register X equals the
      // value of register Y. The right most value must be a 0.
      if ((curr_opcode & 0x00F) != 0) {
        throw InvalidInstruction();
      } else {
        const auto reg_X = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto reg_Y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);
        if (general_reg[reg_X] == general_reg[reg_Y]) {
          pc_reg += 2;
        }
      }
      break;
    case 0x6000:
      // Of form 6XNN. Stores the value NN in the register X
      {
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto value = static_cast<unsigned char>(curr_opcode & 0x00FF);

        general_reg[reg_id] = value;
      }
      break;
    case 0x7000:
      // Of form 7XNN. Adds the value of NN in the register X
      {
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto value = static_cast<unsigned char>(curr_opcode & 0x00FF);
        general_reg[reg_id] += value;
      }
      break;
    case 0x8000:
      switch (curr_opcode & 0x000F) {
        case 0x0000:
          // Of form 8XY0. Stores the value of register Y in register X
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);

            general_reg[reg_x] = general_reg[reg_y];
          }
          break;
        case 0x0001:
          // Of form 8XY1. Stores the result of register X bitwise OR register Y in register X
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);

            general_reg[reg_x] = static_cast<unsigned char>(static_cast<int>(general_reg[reg_x])
                                                            | static_cast<int>(general_reg[reg_y]));
          }
          break;
        case 0x0002:
          // Of form 8XY2. Stores the result of register X bitwise AND register Y in register X
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);

            general_reg[reg_x] = static_cast<unsigned char>(static_cast<int>(general_reg[reg_x])
                                                            & static_cast<int>(general_reg[reg_y]));
          }
          break;
        case 0x0003:
          // Of form 8XY3. Stores the result of register X bitwise XOR register Y in register X
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);

            general_reg[reg_x] = static_cast<unsigned char>(static_cast<int>(general_reg[reg_x])
                                                            ^ static_cast<int>(general_reg[reg_y]));
          }
          break;
        case 0x0004:
          // Of form 8XY4. Adds the value in register Y to the value in register X. If there is an
          // overflow, register F is set to 0x01. Else register F is set to 0x0.
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);
            const auto sum = static_cast<unsigned char>(general_reg[reg_x] + general_reg[reg_y]);

            if (sum < general_reg[reg_x]) {
              general_reg[0xF] = 0x01;
            } else {
              general_reg[0xF] = 0x00;
            }

            general_reg[reg_x] = sum;
          }
          break;
        case 0x0005:
          // Of form 8XY5. Adds the value. Subtracts the value of register Y from register X. Set
          // register F to 0 if borrow occurs, otherwise set register F to 1.
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);

            if (general_reg[reg_x] >= general_reg[reg_y]) {
              general_reg[0xF] = 0x01;
            } else {
              general_reg[0xF] = 0x00;
            }

            general_reg[reg_x] -= general_reg[reg_y];
          }
          break;
        case 0x0006:
          // Of form 8XY6. Stores the value of register Y shifted right one bit in register X.
          // Register F holds the least significant bit of register Y before the shift.
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);

            general_reg[0xF] = static_cast<unsigned char>(general_reg[reg_y] & 0b00000001);

            general_reg[reg_x] = static_cast<unsigned char>(general_reg[reg_y] >> 1);
          }
          break;
        case 0x0007:
          // Of form 8XY7. Stores the value of register Y subtracted by the value of register X in
          // register X. If there is a borrow, register F is set to 0. Else register F is set to 1.
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);
            const auto diff = static_cast<unsigned char>(general_reg[reg_y] - general_reg[reg_x]);

            if (general_reg[reg_y] >= general_reg[reg_x]) {
              general_reg[0xF] = 0x01;
            } else {
              general_reg[0xF] = 0x00;
            }

            general_reg[reg_x] = diff;
          }
          break;
        case 0x000E:
          // Of form 8XYE. Stores the value of register Y shifted left one bit in register X. Stores
          // the most signficant bit of register Y before the shift in register F.
          {
            const auto reg_x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            const auto reg_y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);

            general_reg[0xF] = static_cast<unsigned char>((general_reg[reg_y] & 0b10000000) >> 7);

            general_reg[reg_x] = static_cast<unsigned char>(general_reg[reg_y] << 1);
          }
          break;
        default:
          throw InvalidInstruction();
      }
      break;
    case 0x9000:
      // Must be of form 9XY0. Skips the next instruction if the value of register X does not equal
      // the value of register Y. The right most value must be a 0.
      if ((curr_opcode & 0x00F) != 0) {
        throw InvalidInstruction();
      } else {
        const auto reg_X = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto reg_Y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);
        if (general_reg[reg_X] != general_reg[reg_Y]) {
          pc_reg += 2;
        }
      }
      break;
    case 0xA000:
      // Of form ANNN. Stores memory address NNN in index register
      index_reg = static_cast<unsigned short>(curr_opcode & 0x0FFF);
      break;
    case 0xB000:
      // Of form BNNN. Jump to address NNN + value in register 0
      {
        const auto jump_offset = static_cast<unsigned short>(curr_opcode & 0x0FFF);
        pc_reg = static_cast<unsigned short>(jump_offset + general_reg[0]);
      }
      break;
    case 0xC000:
      // Of form CXNN. Generates a random number from 0 to 255 and masks with NN and stores in
      // register X
      {
        const auto random_num = static_cast<unsigned>(rng(gen));
        const auto mask = static_cast<unsigned>(curr_opcode & 0x00FF);
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);

        general_reg[reg_id] = static_cast<unsigned char>(random_num & mask);
      }
      break;
    case 0xD000:
      // Of form DXYN. Draws a sprite at location (register X value, register Y value) using the
      // sprite data that is a total of N bytes which stored starting at address of the value in
      // register I
      {
        const auto reg_X = static_cast<unsigned char>((curr_opcode & 0x0F00) >> 8);
        const auto reg_Y = static_cast<unsigned char>((curr_opcode & 0x00F0) >> 4);
        const auto height = static_cast<unsigned char>(curr_opcode & 0x000F);
        const auto x_coord = general_reg[reg_X];
        const auto y_coord = general_reg[reg_Y];

        auto collision_flag = false;

        for (auto y = 0; y < height; y++) {
          const auto row_byte = mem.get_value(static_cast<unsigned short>(index_reg + y));
          for (auto x = 0; x < 8; x++) {
            const auto val = static_cast<bool>(row_byte & (0x80 >> x));
            const auto result = graphics.draw_pixel(
                static_cast<size_t>((x_coord + x) % arch::graphics::screen_width),
                static_cast<size_t>((y_coord + y) % arch::graphics::screen_height), val);
            collision_flag = static_cast<bool>(collision_flag || result);
          }
        }

        general_reg[0xF] = collision_flag;

        updated_screen = true;
      }
      break;
    case 0xE000:
      switch (curr_opcode & 0x00FF) {
        case 0x9E:
          // Of form EX9E. Skips the next instruction if the key number in register X is pressed.
          {
            const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            try {
              if (keypad.is_pressed(general_reg[reg_id])) {
                pc_reg += 2;
              }
            } catch (const arch::keypad::InvalidKey&) {
            }
          }
          break;
        case 0xA1:
          // Of form EXA1. Skips the next instruction if the key number in register X is not
          // pressed.
          {
            const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            try {
              if (!keypad.is_pressed(general_reg[reg_id])) {
                pc_reg += 2;
              }
            } catch (const arch::keypad::InvalidKey&) {
            }
          }
          break;
        default:
          throw InvalidInstruction();
      }
      break;
    case 0xF000:
      switch (curr_opcode & 0x00FF) {
        case 0x07:
          // Of form FX07. Store the current value of the delay timer in register X
          {
            const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            general_reg[reg_id] = delay_timer_reg;
          }
          break;
        case 0x0A:
          // Of form FX0A. Wait for key press and store result in register X
          {
            if (keypad.key_pressed) {
              const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
              general_reg[reg_id] = keypad.pressed_key;
            } else {
              pc_reg -= 2;
            }
          }
          break;
        case 0x15:
          // Of form FX15. Set the delay timer to the value in register X
          {
            const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            delay_timer_reg = general_reg[reg_id];
          }
          break;
        case 0x18:
          // Of form FX1E. Set the sound timer to the value in register X
          {
            const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
            sound_timer_reg = general_reg[reg_id];
          }
          break;
        case 0x1E:
          // Of form FX1E. Adds the value in register X to register I
          {
            const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);

            index_reg += general_reg[reg_id];
          }
          break;
        case 0x29:
          // Of form FX29. Set the value of register I to the address of the sprite that represents
          // the hexadecimal digit stored in register X.
          {
            const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);

            index_reg = (general_reg[reg_id] & 0x0F) * 5;
          }
          break;
        case 0x33:
          // Of form FX33. Set the value stored in register X in binary coded. The hundredth digit
          // is placed at address stored in register I, the tenth digit is placed at 1 + address
          // stored in register I and the ones digit is placed at 2 + address stored in register I.
          {
            const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);

            // Yay floor division
            // truncates the last 2 digits
            mem.set_value(index_reg, general_reg[reg_id] / 100);
            // truncates the last digit and then truncates the first digit of result
            mem.set_value(index_reg + 1, (general_reg[reg_id] / 10) % 10);
            // truncates the first digit and then truncates the first digit of result
            mem.set_value(index_reg + 2, (general_reg[reg_id] % 100) % 10);
          }
          break;
        case 0x55:
          // Of form FX55. Stores the values from register 0 to register X inclusive starting at the
          // address in register I. Register I is then set to I + X + 1 after the operation.
          {
            const auto x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);

            for (auto reg = 0; reg <= x; reg++) {
              mem.set_value(index_reg + reg, general_reg[reg]);
            }
            index_reg = static_cast<unsigned short>(index_reg + x + 1);
          }
          break;
        case 0x65:
          // Of form FX65. Stores values in registers 0 to X inclusive with values in memory
          // starting from the address in register I. Register I is then set to I + X + 1 after the
          // operation.
          {
            const auto x = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);

            for (auto reg = 0; reg <= x; reg++) {
              general_reg[reg] = mem.get_value(index_reg + reg);
            }
            index_reg = static_cast<unsigned short>(index_reg + x + 1);
          }

          break;
        default:
          throw InvalidInstruction();
      }
      break;
    default:
      // This is actually technically impossible to reach
      throw InvalidInstruction();
  }
}

unsigned char arch::CPU::get_general_reg(size_t reg_id) const {
  if (reg_id >= num_general_reg) {
    throw InvalidRegisterID();
  } else {
    return general_reg[reg_id];
  }
}

void arch::CPU::set_general_reg(size_t reg_idx, unsigned char value) {
  if (reg_idx >= num_general_reg) {
    throw InvalidRegisterID();
  } else {
    general_reg[reg_idx] = value;
  }
}

unsigned short arch::CPU::get_stack_pointer() const { return sp_reg; }

void arch::CPU::set_stack_pointer(unsigned char value) {
  if (value >= stack_size) {
    throw InvalidStackPointerValue();
  } else {
    sp_reg = value;
  }
}

unsigned short arch::CPU::get_stack() const { return stack[sp_reg]; }

void arch::CPU::set_stack(unsigned short value) { stack[sp_reg] = value; }
