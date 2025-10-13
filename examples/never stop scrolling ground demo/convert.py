import os
from PIL import Image
import glob

def crop_pngs(input_dir=None, output_dir=None, width=16, height=16):
    """
    Crop all PNG files in a directory to specified dimensions
    """
    
    # Set input directory
    if input_dir is None:
        input_dir = os.getcwd()
    
    # Set output directory
    if output_dir is None:
        output_dir = input_dir
    else:
        os.makedirs(output_dir, exist_ok=True)
    
    # Find all PNG files
    png_pattern = os.path.join(input_dir, "*.png")
    png_files = glob.glob(png_pattern)
    
    if not png_files:
        print(f"No PNG files found in {input_dir}")
        return
    
    print(f"Found {len(png_files)} PNG files to process...")
    
    # Process each PNG file
    for png_file in png_files:
        # Skip icon.png
        if os.path.basename(png_file).lower() == 'icon.png':
            print(f"Skipping: {os.path.basename(png_file)}")
            continue
            
        try:
            # Open image
            with Image.open(png_file) as img:
                # Convert to RGB (no transparency needed)
                if img.mode != 'RGB':
                    img = img.convert('RGB')
                
                # Get original dimensions
                orig_width, orig_height = img.size
                
                # Calculate crop area (center crop)
                left = (orig_width - width) // 2
                top = (orig_height - height) // 2
                right = left + width
                bottom = top + height
                
                # Ensure crop area is within image bounds
                left = max(0, left)
                top = max(0, top)
                right = min(orig_width, right)
                bottom = min(orig_height, bottom)
                
                # Crop the image
                cropped_img = img.crop((left, top, right, bottom))
                
                # If the cropped image is smaller than target, resize to fit
                if cropped_img.size != (width, height):
                    cropped_img = cropped_img.resize((width, height), Image.Resampling.LANCZOS)
                
                # Determine output filename
                if output_dir == input_dir:
                    output_path = png_file
                else:
                    filename = os.path.basename(png_file)
                    output_path = os.path.join(output_dir, filename)
                
                # Save as RGB
                cropped_img.save(output_path, "PNG")
                print(f"Processed: {os.path.basename(png_file)} ({orig_width}x{orig_height} -> {width}x{height})")
                
        except Exception as e:
            print(f"Error processing {png_file}: {str(e)}")
    
    print("Processing completed!")

def main():
    """Main function with example usage"""
    
    # Process PNGs in current directory (overwrite originals)
    crop_pngs()
    
    # Alternative: Process from one directory to another
    # crop_pngs(
    #     input_dir="./source_images",
    #     output_dir="./processed_images",
    #     width=38,
    #     height=58
    # )

if __name__ == "__main__":
    main()