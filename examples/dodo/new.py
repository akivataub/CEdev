import os
import subprocess
import glob
import yaml

CONVERT_NAME = 'sprites'

def get_current_counter(lines):
    for line in lines:
        if 'num_sprites =' in line:
            return int(line.split('=')[1].strip()[:-1]) + 1
    return 3  # default

def migrate_yaml():
    with open('convimg.yaml', 'r') as f:
        content = f.read()

    # Replace tabs with spaces for parsing
    content = content.replace('\t', '  ')

    # Fix invalid images line if present
    lines = content.splitlines()
    new_lines = []
    for line in lines:
        stripped = line.strip()
        if stripped.startswith('images:') and '-' in stripped and not stripped.endswith(':'):
            indent = line[:line.find('images:')]
            new_lines.append(indent + 'images:')
            images_str = stripped[len('images:'):].strip()
            images = images_str.split()
            for img in images:
                if img.strip():
                    clean_img = img.strip().lstrip('-')
                    new_lines.append(indent + '  - ' + clean_img)
        else:
            new_lines.append(line)

    content = '\n'.join(new_lines)
    data = yaml.safe_load(content)

    # Find all old bX converts and collect images
    old_images = []
    new_converts = []
    for c in data.get('converts', []):
        name = c.get('name')
        if name.startswith('b') and name[1:].isdigit():
            images = c.get('images', [])
            if isinstance(images, str):
                images = [img.strip().lstrip('-') for img in images.split() if img.strip()]
            old_images += images
        else:
            new_converts.append(c)
    data['converts'] = new_converts

    # Filter existing files
    old_images = [img for img in old_images if os.path.exists(img)]

    # Add to sprites
    convert_block = None
    for c in data['converts']:
        if c.get('name') == CONVERT_NAME:
            convert_block = c
            break
    if convert_block is None:
        convert_block = {
            'name': CONVERT_NAME,
            'palette': 'global_palette',
            'transparent-color-index': 0,
            'bpp': 8,
            'images': old_images
        }
        data['converts'].append(convert_block)
    else:
        existing_images = convert_block.get('images', [])
        if isinstance(existing_images, str):
            existing_images = [img.strip().lstrip('-') for img in existing_images.split() if img.strip()]
        all_images = existing_images + old_images
        unique_images = list(set(all_images))
        convert_block['images'] = [img for img in unique_images if os.path.exists(img)]

    # Remove old outputs for bX and duplicates
    new_outputs = []
    seen_convs = set()
    for o in data.get('outputs', []):
        convs = tuple(sorted(o.get('converts', [])))
        if convs in seen_convs:
            continue
        seen_convs.add(convs)
        if len(convs) == 1 and convs[0].startswith('b') and convs[0][1:].isdigit():
            continue
        new_outputs.append(o)
    data['outputs'] = new_outputs

    # Ensure output for sprites and global_palette
    output_exists = False
    for o in data['outputs']:
        if CONVERT_NAME in o.get('converts', []):
            output_exists = True
            break
    if not output_exists:
        new_output = {
            'type': 'c',
            'include-file': 'gfx.h',
            'converts': [CONVERT_NAME]
        }
        data['outputs'].append(new_output)

    # Remove other convert blocks like my_sprites
    data['converts'] = [c for c in data['converts'] if c.get('name') == CONVERT_NAME]

    with open('convimg.yaml', 'w') as f:
        yaml.dump(data, f, default_flow_style=False, sort_keys=False)

    # Post-process to use 3 tabs for images lines
    with open('convimg.yaml', 'r') as f:
        lines = f.readlines()
    with open('convimg.yaml', 'w') as f:
        in_images = False
        for line in lines:
            stripped = line.strip()
            if stripped == 'images:':
                in_images = True
                f.write(line)
            elif in_images and stripped.startswith('- '):
                line = '\t\t\t' + stripped + '\n'
                f.write(line)
            else:
                in_images = False
                f.write(line)

def migrate_sprites(lines):
    for i, line in enumerate(lines):
        if 'gfx_sprite_t *sprites[MAX_SPRITES] =' in line:
            parts = line.split('=')[1].strip()[1:-2]
            items = [p.strip() for p in parts.split(',')]
            new_items = []
            for item in items:
                if item == 'NULL':
                    new_items.append(item)
                elif item.startswith(CONVERT_NAME + '_'):
                    new_items.append(item)
                elif item.startswith('my_sprites_'):
                    new_items.append(item.replace('my_sprites_', CONVERT_NAME + '_'))
                else:
                    new_items.append(f'{CONVERT_NAME}_{item}')
            new_array = ' { ' + ', '.join(new_items) + ' };'
            lines[i] = 'gfx_sprite_t *sprites[MAX_SPRITES] =' + new_array + '\n'
            break

def add_to_convimg(name):
    with open('convimg.yaml', 'r') as f:
        content = f.read()

    content = content.replace('\t', '  ')

    lines = content.splitlines()
    new_lines = []
    for line in lines:
        stripped = line.strip()
        if stripped.startswith('images:') and '-' in stripped and not stripped.endswith(':'):
            indent = line[:line.find('images:')]
            new_lines.append(indent + 'images:')
            images_str = stripped[len('images:'):].strip()
            images = images_str.split()
            for img in images:
                if img.strip():
                    clean_img = img.strip().lstrip('-')
                    new_lines.append(indent + '  - ' + clean_img)
        else:
            new_lines.append(line)

    content = '\n'.join(new_lines)
    data = yaml.safe_load(content)
    
    # Add to sprites images
    convert_block = None
    for c in data.get('converts', []):
        if c.get('name') == CONVERT_NAME:
            convert_block = c
            break
    
    if convert_block is None:
        new_convert = {
            'name': CONVERT_NAME,
            'palette': 'global_palette',
            'transparent-color-index': 0,
            'bpp': 8,
            'images': [f'{name}.png']
        }
        data['converts'] = data.get('converts', []) + [new_convert]
    else:
        existing_images = convert_block.get('images', [])
        if isinstance(existing_images, str):
            existing_images = [img.strip().lstrip('-') for img in existing_images.split() if img.strip()]
        existing_images.append(f'{name}.png')
        convert_block['images'] = existing_images
    
    # Ensure output exists
    output_exists = False
    for o in data.get('outputs', []):
        if CONVERT_NAME in o.get('converts', []):
            output_exists = True
            break
    if not output_exists:
        new_output = {
            'type': 'c',
            'include-file': 'gfx.h',
            'converts': [CONVERT_NAME]
        }
        data['outputs'] = data.get('outputs', []) + [new_output]
    
    with open('convimg.yaml', 'w') as f:
        yaml.dump(data, f, default_flow_style=False, sort_keys=False)
    
    # Post-process to use 3 tabs for images lines
    with open('convimg.yaml', 'r') as f:
        lines = f.readlines()
    with open('convimg.yaml', 'w') as f:
        in_images = False
        for line in lines:
            stripped = line.strip()
            if stripped == 'images:':
                in_images = True
                f.write(line)
            elif in_images and stripped.startswith('- '):
                line = '\t\t\t' + stripped + '\n'
                f.write(line)
            else:
                in_images = False
                f.write(line)

def remove_from_convimg(name):
    with open('convimg.yaml', 'r') as f:
        content = f.read()

    content = content.replace('\t', '  ')

    lines = content.splitlines()
    new_lines = []
    for line in lines:
        stripped = line.strip()
        if stripped.startswith('images:') and '-' in stripped and not stripped.endswith(':'):
            indent = line[:line.find('images:')]
            new_lines.append(indent + 'images:')
            images_str = stripped[len('images:'):].strip()
            images = images_str.split()
            for img in images:
                if img.strip():
                    clean_img = img.strip().lstrip('-')
                    new_lines.append(indent + '  - ' + clean_img)
        else:
            new_lines.append(line)

    content = '\n'.join(new_lines)
    data = yaml.safe_load(content)
    
    # Remove from images in sprites
    for c in data.get('converts', []):
        if c.get('name') == CONVERT_NAME:
            images = c.get('images', [])
            if isinstance(images, str):
                images = [img.strip().lstrip('-') for img in images.split() if img.strip()]
            c['images'] = [img for img in images if img != f'{name}.png']
            break
    
    with open('convimg.yaml', 'w') as f:
        yaml.dump(data, f, default_flow_style=False, sort_keys=False)
    
    # Post-process to use 3 tabs for images lines
    with open('convimg.yaml', 'r') as f:
        lines = f.readlines()
    with open('convimg.yaml', 'w') as f:
        in_images = False
        for line in lines:
            stripped = line.strip()
            if stripped == 'images:':
                in_images = True
                f.write(line)
            elif in_images and stripped.startswith('- '):
                line = '\t\t\t' + stripped + '\n'
                f.write(line)
            else:
                in_images = False
                f.write(line)

def add_include(lines, name):
    # No need for separate includes
    pass

def add_to_sprites(lines, name):
    for i, line in enumerate(lines):
        if 'gfx_sprite_t *sprites[MAX_SPRITES] =' in line:
            parts = line.split('=')[1].strip()[1:-2]  # remove { and };
            items = [p.strip() for p in parts.split(',')]
            for j in range(len(items)):
                if items[j] == 'NULL':
                    items.insert(j, f'{CONVERT_NAME}_{name}')
                    break
            new_array = ' { ' + ', '.join(items) + ' };'
            lines[i] = 'gfx_sprite_t *sprites[MAX_SPRITES] =' + new_array + '\n'
            break

def update_num_sprites(lines, delta=1):
    for i, line in enumerate(lines):
        if 'num_sprites =' in line:
            num = int(line.split('=')[1].strip()[:-1])
            new_num = num + delta
            lines[i] = f'int num_sprites = {new_num};\n'
            break

def resize_image(file, name):
    subprocess.call(['magick', file, '-resize', '155x105^>', f'{name}.png'])

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

def remove_include(lines, name):
    # No separate includes
    pass

def remove_from_sprites(lines, name):
    for i, line in enumerate(lines):
        if 'gfx_sprite_t *sprites[MAX_SPRITES] =' in line:
            parts = line.split('=')[1].strip()[1:-2]
            items = [p.strip() for p in parts.split(',')]
            full_name = f'{CONVERT_NAME}_{name}'
            if full_name in items:
                items.remove(full_name)
            new_array = ' { ' + ', '.join(items) + ' };'
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

        migrate_yaml()

        with open('src/main.c', 'r') as f:
            lines = f.readlines()

        # Remove old separate includes
        lines = [l for l in lines if not (l.startswith('#include "b') and l.endswith('.h"\n') and 'gfx.h' not in l)]

        migrate_sprites(lines)

        if choice in ['1', '3']:
            horiz = input("Horizontal or Vertical layout (h/v)? ").lower() == 'h'
            set_layout(lines, horiz)
            pair = input("View mode pair or single (p/s)? ").lower() == 'p'
            set_view_mode(lines, pair)

            counter = get_current_counter(lines)

            if choice == '1':
                num = int(input("How many photos to add? "))
                files = []
                for _ in range(num):
                    file = input("Enter photo file path: ")
                    files.append(file)
            else:
                files = glob.glob('*.png') + glob.glob('*.jpg')
                files = [f for f in files if not f.startswith('b')]

            for file in files:
                name = f'b{counter}'
                resize_image(file, name)
                add_to_convimg(name)
                add_to_sprites(lines, name)
                update_num_sprites(lines, 1)
                counter += 1
                print(f"Added {name}")

        elif choice == '2':
            num = int(input("How many photos to remove? "))
            for _ in range(num):
                bname = input("Enter b name to remove (e.g., b3): ")
                remove_from_convimg(bname)
                remove_from_sprites(lines, bname)
                update_num_sprites(lines, -1)
                print(f"Removed {bname}")

        with open('src/main.c', 'w') as f:
            f.writelines(lines)

        print("Changes saved. Running go.bat...")
        subprocess.call('go.bat', shell=True)

if __name__ == "__main__":
    main()