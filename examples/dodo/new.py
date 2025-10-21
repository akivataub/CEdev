import os
import subprocess
import glob

def get_current_counter(lines):
    for line in lines:
        if 'num_sprites =' in line:
            return int(line.split('=')[1].strip()[:-1]) + 1
    return 3  # default

def add_to_convimg(name):
    with open('convimg.yaml', 'a') as f:
        f.write(f'  - name: {name}\n')
        f.write('    palette: global_palette\n')
        f.write('    images:\n')
        f.write(f'      - {name}.png\n')

def add_include(lines, name):
    last_include_idx = -1
    for i, line in enumerate(lines):
        if line.startswith('#include "b') and line.endswith('.h"\n'):
            last_include_idx = i
    if last_include_idx != -1:
        lines.insert(last_include_idx + 1, f'#include "{name}.h"\n')

def add_to_sprites(lines, name):
    for i, line in enumerate(lines):
        if 'gfx_sprite_t *sprites[MAX_SPRITES] =' in line:
            parts = line.split('=')[1].strip()[1:-2]  # remove { and };
            items = [p.strip() for p in parts.split(',')]
            for j, item in enumerate(items):
                if item == 'NULL':
                    items.insert(j, name)
                    break
            new_array = '{' + ', '.join(items) + '};'
            lines[i] = 'gfx_sprite_t *sprites[MAX_SPRITES] = ' + new_array + '\n'
            break

def update_num_sprites(lines, delta=1):
    for i, line in enumerate(lines):
        if 'num_sprites =' in line:
            num = int(line.split('=')[1].strip()[:-1])
            new_num = num + delta
            lines[i] = f'int num_sprites = {new_num};\n'
            break

def resize_image(file, name):
    subprocess.call(['magick', 'convert', file, '-resize', '155x105^>', f'{name}.png'])

def set_layout(lines, is_horiz):
    for i, line in enumerate(lines):
        if 'is_horizontal =' in line:
            lines[i] = f'bool is_horizontal = {"true" if is_horiz else "false"};\n'
            break

def set_view_mode(lines, is_pair):
    for i, line in enumerate(lines):
        if 'view_mode =' in line:
            lines[i] = f'int view_mode = {0 if is_pair else 1}; // 0: pair, 1: single\n'
            break

def remove_from_convimg(name):
    with open('convimg.yaml', 'r') as f:
        lines = f.readlines()
    start = -1
    for i, line in enumerate(lines):
        if f'  - name: {name}' in line:
            start = i
            break
    if start != -1:
        del lines[start:start+4]
    with open('convimg.yaml', 'w') as f:
        f.writelines(lines)

def remove_include(lines, name):
    for i, line in enumerate(lines):
        if line == f'#include "{name}.h"\n':
            del lines[i]
            break

def remove_from_sprites(lines, name):
    for i, line in enumerate(lines):
        if 'gfx_sprite_t *sprites[MAX_SPRITES] =' in line:
            parts = line.split('=')[1].strip()[1:-2]
            items = [p.strip() for p in parts.split(',')]
            if name in items:
                items.remove(name)
            new_array = '{' + ', '.join(items) + '};'
            lines[i] = 'gfx_sprite_t *sprites[MAX_SPRITES] = ' + new_array + '\n'
            break

def main():
    while True:
        print("1. Add photos (specify which ones)")
        print("2. Remove photos")
        print("3. Scan and add all photos in dir (auto add new .png/.jpg)")
        print("4. Exit")
        choice = input("Choose option: ")
        if choice == '4':
            break

        with open('src/main.c', 'r') as f:
            lines = f.readlines()

        if choice in ['1', '3']:
            horiz = input("Horizontal or Vertical layout (h/v)? ").lower() == 'h'
            set_layout(lines, horiz)
            pair = input("View mode pair or single (p/s)? ").lower() == 'p'
            set_view_mode(lines, pair)
            # Note: photos per page is 2 for pair, 1 for single; pages calculated based on total photos

            counter = get_current_counter(lines)

            if choice == '1':
                num = int(input("How many photos to add? "))
                files = []
                for _ in range(num):
                    file = input("Enter photo file path: ")
                    files.append(file)
            else:  # '3'
                files = glob.glob('*.png') + glob.glob('*.jpg')
                files = [f for f in files if not f.startswith('b')]

            for file in files:
                name = f'b{counter}'
                resize_image(file, name)
                add_to_convimg(name)
                add_include(lines, name)
                add_to_sprites(lines, name)
                update_num_sprites(lines, 1)
                counter += 1
                print(f"Added {name}")

        elif choice == '2':
            num = int(input("How many photos to remove? "))
            for _ in range(num):
                bname = input("Enter b name to remove (e.g., b3): ")
                remove_from_convimg(bname)
                remove_include(lines, bname)
                remove_from_sprites(lines, bname)
                update_num_sprites(lines, -1)
                print(f"Removed {bname}")

        with open('src/main.c', 'w') as f:
            f.writelines(lines)

        print("Changes saved. Run convimg and recompile.")

if __name__ == "__main__":
    main()