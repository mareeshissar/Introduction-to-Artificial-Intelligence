import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib import style
style.use('ggplot')
import numpy as np
import cv2
import math
import random
from PIL import Image


X = cv2.imread('scarlet_tanager.jpg',  cv2.IMREAD_COLOR)     ## read the image ( M * N * 3)
img = Image.fromarray(X, 'RGB')
img.save('my1.png')
img.show()

#print(X.shape)
#print(X[1])

X = X.reshape((-1,3))
#print(X.shape)

print('********reshape to original*********')
X_new = X.reshape((240,420,-1))
#print(X_new[1])
#print(X_new.shape)

class K_Means:
    def __init__(self, k=29, tol=0.001, max_iter=300):
        self.k = k                                      ## number of clusters
        self.tol = tol                                  ## the tol. if the centroids do not move by 0.001, stop
        self.max_iter = max_iter                        ## number of iterations

    def fit(self,data):

        self.centroids = []                             ##initially the centroids dictionary is empty

        for i in range(self.k):
            val = random.choice(data)
            self.centroids.append(val)                 ## initialize the centroids with first K pixels in the image. Can also randomize it as well


        for i in range(self.max_iter):                  ## start to run for max-iterations

            print('******start of Kmeans*****')
            self.classifications = {}                   ## always begin by new classifications. That is the number of pixels for each centroid as a dictionary

            for i in range(self.k):
                self.classifications[i] = []            ## the intial classification is always empty, and then we get K classifications

            for pixel in data:
                distances = []
                for centroid in self.centroids:
                    distance = math.sqrt( 2*( pixel[0] - centroid[0])**2 + 4*(pixel[1] - centroid[1])**2 + 3*(pixel[2] - centroid[2])**2 )
                    distances.append(distance)

                classification = distances.index(min(distances))
                # print('**classification**')
                # print(classification)
                self.classifications[classification].append(pixel)

            print('***classifications******')
            #print(self.classifications[2])
            #print(self.classifications)

            print('***printing self-centroids****')
            prev_centroids = self.centroids[:]
            #print(self.centroids)
            #print(prev_centroids)


            for classification in self.classifications:
                self.centroids[classification] = np.around(np.average(self.classifications[classification],axis=0))

            #print(type(self.centroids))


            print('***after classifications*****')
            #print(self.centroids)

            check_tol = True

            size = len(prev_centroids)
            #print(size)
            #print(len(self.centroids))
            for c in range(0,size):
                #print('****start of checking for tolerance****')
                original_centroid = prev_centroids[c]
                current_centroid = self.centroids[c]

                distance_moved = math.sqrt( ( original_centroid[0] - current_centroid[0])**2 + (original_centroid[1] - current_centroid[1])**2 + (original_centroid[2] - current_centroid[2])**2 )
                original_dist = math.sqrt( (original_centroid[0])**2 + (original_centroid[1])**2 + (original_centroid[2])**2 )

                if (distance_moved - original_dist)/original_dist*100.0 > self.tol:
                    check_tol = False

            if check_tol:
                break

clf = K_Means()

clf.fit(X)

#print(clf.centroids)
#print(len(clf.centroids))



#print(type(X[1]))
#print(type(clf.centroids[1]))

# 100800 * 9 matrix declare with all 9 col = 0
Y = [[0 for i in range(29)] for j in range(100800)]
counter = 0   #for numbering the pixel

num_of_row = 240
num_of_col = 420

for pixel in X:
    distances = []
    for centroid in clf.centroids:
        distance = math.sqrt( ( pixel[0] - centroid[0])**2 + (pixel[1] - centroid[1])**2 + (pixel[2] - centroid[2])**2 )
        distances.append(distance)
    ind = distances.index(min(distances))
    Y[counter][ind] = 1
    pixel[:] = clf.centroids[ind][:]
    counter=counter+1


X = X.reshape((240,420,-1))
#print(X[1])
img2 = Image.fromarray(X, 'RGB')
img2.save('my.png')
img2.show()


# Load image
image_bgr = cv2.imread('scarlet_tanager.jpg', cv2.IMREAD_COLOR)
image_gray = [[0 for i in range(420)] for j in range(240)]

# Convert to RGB
image_rgb = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2RGB)


# Convert to grayscale
print(image_rgb[0][0])
for x in range(num_of_row):
    for y in range(num_of_col):
        image_gray[x][y]=round(image_rgb[x][y][0]*0.21 + image_rgb[x][y][1]*0.72 + image_rgb[x][y][2]*0.07)


x_gray=np.asarray(image_gray)
x=x_gray
#print(x.reshape(-1,1))



k=29  #number of clusters
x_new = [[0 for i in range(9)] for j in range( num_of_row * num_of_col )]


for i in range(0,num_of_row):
    for j in range(0, num_of_col):
        if(i-1>=0 and j-1>=0):
            x_new[(num_of_col * i) + j][0] = x[i - 1][j - 1]
        else:
            x_new[(num_of_col * i) + j][0] = 0

        if(i-1>=0):
            x_new[(num_of_col * i) + j][1] = x[i - 1][j]
        else:
            x_new[(num_of_col * i) + j][1] = 0

        if(i-1>=0 and j+1<num_of_col):
            x_new[ (num_of_col* i ) + j][2]=x[i-1][j+1]
        else:
            x_new[(num_of_col * i) + j][2] = 0

        if(j-1>=0):
            x_new[ (num_of_col* i ) + j][3]=x[i][j-1]
        else:
            x_new[(num_of_col * i) + j][3] = 0

        x_new[ (num_of_col* i ) + j][4]=x[i][j]

        if(j+1<num_of_col):
            x_new[ (num_of_col* i ) + j][5]=x[i][j+1]
        else:
            x_new[(num_of_col * i) + j][5] = 0

        if(i+1<num_of_row and j-1>=0):
            x_new[ (num_of_col* i ) + j][6]=x[i+1][j-1]
        else:
            x_new[(num_of_col * i) + j][6] = 0

        if(i+1<num_of_row):
            x_new[ (num_of_col* i ) + j][7]=x[i+1][j]
        else:
            x_new[(num_of_col * i) + j][7] = 0

        if(i+1<num_of_row and j+1<num_of_col):
            x_new[ (num_of_col* i ) + j][8]=x[i+1][j+1]
        else:
            x_new[(num_of_col * i) + j][8] = 0

x_new=np.asarray(x_new)
print("debug")
x_new=x_new/255
print(x_new.shape)
print(x_new[0])

#adding bias
#bias=np.ones( ( ((num_of_row) * (num_of_col)),1 ), dtype=int)
#x_new=np.append(x_new,bias,axis=1)
#x_new=x_new.astype(int)
print(x_new)

y_new=Y #one hot encoded output
y_new=np.asarray(y_new)
print("debug y new")
print(y_new.shape)
print(y_new[0])

learning_rate=0.2
num_of_hidden=50
num_of_iter=100

def act_funct(x):
    return 1/(1+np.exp(-x))

def slope_act_funct(x):
    return x*(1-x)

def train(x_new,y_new,num_of_hidden,num_of_iter):

    w1 = 2 * np.random.random_sample((9, num_of_hidden)) - 1
    # print("Initial weights after input layer:")
    # print(w1)

    w2 = 2 * np.random.random_sample((num_of_hidden, k)) - 1
    # print("Initial weights after hidden layer:")
    # print(w2)

    for j in range(num_of_iter):
        l1 = np.matmul(x_new, w1)
        l1 = act_funct(l1)  # output of the hidden layer after activation

        l2 = np.matmul(l1, w2)

        l2 = act_funct(l2)  # final output after activation
        print(y_new[0])
        print(l2[0])

        loss = - (y_new * np.log(l2) + (1 - y_new) * np.log(1 - l2))
        print("Loss after " + str(j) + " iterations: ", np.mean(loss))

        delta2 = (y_new - l2) * slope_act_funct(l2)  # element-wise multiplication

        # updating weights for output layer
        w2 = w2 + learning_rate * np.matmul(l1.T, delta2)

        a = np.matmul(delta2, w2.T)
        l1 = slope_act_funct(l1)
        delta1 = a * l1

        # updating weights for hidden layer
        w1 = w1 + learning_rate * np.matmul(x_new.T, delta1)
    return w1,w2,l2


def results(l2,y_new):
    for i in range((num_of_row ) * (num_of_col)):
        max_row = np.amax(l2[i])
        for j in range(k):
            if (l2[i][j] != max_row):
                l2[i][j] = 0
            else:
                l2[i][j] = 1

    print(y_new - l2)

    print("Accuracy: ", 1 - (np.count_nonzero(y_new - l2) / 2) / ((num_of_row) * (num_of_col)))

w1,w2,l2=train(x_new,y_new,num_of_hidden,num_of_iter)

results(l2,y_new)
