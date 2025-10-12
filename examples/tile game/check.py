from PIL import Image
import sys
import os

def main():
    if len(sys.argv) < 2:
        print("Usage: python check.py image1.png image2.png ...")
        return
    
    output_file = "color_output.txt"
    
    # Clear output file
    open(output_file, 'w').close()
    
    files = sys.argv[1:]
    valid_files = []
    
    for file_path in files:
        if os.path.exists(file_path):
            valid_files.append(file_path)
        else:
            print(f"File not found: {file_path}")
    
    if not valid_files:
        print("No valid files provided!")
        return
    
    # Collect all unique colors from all images
    all_colors = set()
    
    print(f"Processing {len(valid_files)} images...")
    for file_path in valid_files:
        try:
            image = Image.open(file_path)
            image = image.convert("RGB")
            width, height = image.size
            
            print(f"  Reading: {file_path}")
            
            for x in range(width):
                for y in range(height):
                    pixel = image.getpixel((x, y))
                    all_colors.add(pixel)
                    
        except Exception as e:
            print(f"Error processing {file_path}: {e}")
    
    # Write all unique colors only once with proper tabs
    with open(output_file, 'a') as f:
        f.write("# All Unique Colors from {} images\n".format(len(valid_files)))
        for index, color in enumerate(sorted(all_colors)):
            r, g, b = color
            f.write("	- color: {{index: {}, r: {}, g: {}, b: {}}}\n".format(index, r, g, b))
    
    print(f"\nTotal unique colors found: {len(all_colors)}")
    print(f"All colors written to: {output_file}")

if __name__ == "__main__":
    main()