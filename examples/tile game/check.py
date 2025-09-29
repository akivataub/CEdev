from PIL import Image

im = Image.open("tile_back.png").convert("RGBA")
colors = im.getcolors(maxcolors=256)

for count, color in colors:
    print(f"{color} : {count} pixels")
