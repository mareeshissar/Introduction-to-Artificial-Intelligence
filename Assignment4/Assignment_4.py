import cv2
import numpy as np
from matplotlib import pyplot as plt


# Load image
image_bgr = cv2.imread('images/scarlet_tanager.jpg', cv2.IMREAD_COLOR)

image_gray = [[0 for i in range(420)] for j in range(240)]

# Convert to RGB
image_rgb = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2RGB)


print(image_rgb[0][0])
for x in range(240):
    for y in range(420):
        image_gray[x][y]=round(image_rgb[x][y][0]*0.21 + image_rgb[x][y][1]*0.72 + image_rgb[x][y][2]*0.07)

print(image_gray[0][0])

# Show image
plt.imshow(image_gray,cmap='gray'), plt.axis("off")
plt.show()


image = cv2.imread('images/scarlet_tanager.jpg', cv2.IMREAD_GRAYSCALE)
# Show image
plt.imshow(image,cmap='gray'), plt.axis("off")
plt.show()

print(image[0][0])









