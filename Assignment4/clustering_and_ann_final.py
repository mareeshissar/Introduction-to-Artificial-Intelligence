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

X = X.reshape((-1,3))    # 10800 * 3
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

num_of_row = 240
num_of_col = 420
Y = [[0 for i in range(29)] for j in range(num_of_row * num_of_col)]            ## 2-D list
print('type for y')
print(type(Y))
counter = 0   #for numbering the pixel



for pixel in X:
    distances = []
    for centroid in clf.centroids:
        distance = math.sqrt( ( pixel[0] - centroid[0])**2 + (pixel[1] - centroid[1])**2 + (pixel[2] - centroid[2])**2 )
        distances.append(distance)
    ind = distances.index(min(distances))
    Y[counter][ind] = 1
    pixel[:] = clf.centroids[ind][:]
    counter=counter+1

print('values of Y[1] and Y[2]')
print(Y[1])             ### rows*col * 29
print(Y[2])



X = X.reshape((240,420,-1))
#print(X[1])
img2 = Image.fromarray(X, 'RGB')
img2.save('my.png')
img2.show()


##### X final image
##### X ( 3D 240 * 420 * 3)


# Load image
image_bgr = cv2.imread('scarlet_tanager.jpg', cv2.IMREAD_COLOR)

# Convert to RGB
image_rgb = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2RGB)


# Convert to grayscale

print('print image_rgb[0] and image_rgb[1] before grayscale')
print(image_rgb[0][0])
print(image_rgb[1][1])

X_gray_new = [[0 for i in range(num_of_col)] for j in range( num_of_row)]
for x in range(num_of_row):
    for y in range(num_of_col):
        X_gray_new[x][y]=round(image_rgb[x][y][0]*0.21 + image_rgb[x][y][1]*0.72 + image_rgb[x][y][2]*0.07)


print('print X_gray_new[0] and X_gray_new[1] after grayscale')

print(X_gray_new[0][0])
print(X_gray_new[1][1])


x=np.asarray(X_gray_new)
print('print x shape')
print(x.shape)




k=29  #number of clusters
x_new = [[0 for i in range(9)] for j in range( num_of_row * num_of_col )]


## 2D list dimensions (rows*col) * 9
### input of nueral

pixel_num = 0
for i in range(0,num_of_row):
    for j in range(0, num_of_col):

        if(i-1>=0 and j-1>=0):
            x_new[pixel_num][0] = x[i - 1][j - 1]
        else:
            x_new[pixel_num][0] = 0

        if(i-1>=0):
            x_new[pixel_num][1] = x[i - 1][j]
        else:
            x_new[pixel_num][1] = 0

        if(i-1>=0 and j+1<num_of_col):
            x_new[pixel_num][2]=x[i-1][j+1]
        else:
            x_new[pixel_num][2] = 0

        if(j-1>=0):
            x_new[ pixel_num][3]=x[i][j-1]
        else:
            x_new[pixel_num][3] = 0

        x_new[ pixel_num][4]=x[i][j]

        if(j+1<num_of_col):
            x_new[ pixel_num][5]=x[i][j+1]
        else:
            x_new[pixel_num][5] = 0

        if(i+1<num_of_row and j-1>=0):
            x_new[ pixel_num][6]=x[i+1][j-1]
        else:
            x_new[pixel_num][6] = 0

        if(i+1<num_of_row):
            x_new[ pixel_num][7]=x[i+1][j]
        else:
            x_new[pixel_num][7] = 0

        if(i+1<num_of_row and j+1<num_of_col):
            x_new[pixel_num][8]=x[i+1][j+1]
        else:
            x_new[pixel_num][8] = 0

        pixel_num = pixel_num + 1



x_new=np.asarray(x_new)


print("debug")
x_new = x_new/255
print('x_new shape:')
print(x_new.shape)

print('print x_new values')
print(x_new[0])
print(x_new[1])
print(x_new[2])


y_new=Y #one hot encoded output
y_new=np.asarray(y_new)
print("debug y new")

print('y_new shapes and values')
print(y_new.shape)
print(y_new[0])
print(y_new[1])
print(y_new[2])

learning_rate=10e-6
num_of_hidden=60
num_of_iter=1000
target_error = 0.1


def act_funct(x):
    return 1/( 1+np.exp(-x) )


def softmax_act_funct(x):
    y=np.exp(x.T) / np.sum(np.exp(x.T), axis=0)
    return y.T


def slope_act_funct(x):
    return x*(1-x)

def train(x_new,y_new,num_of_hidden,num_of_iter, target_error):

    w1 = 2 * np.random.random((9, num_of_hidden)) - 1
    #print('w1 shape')
    #print(w1.shape)
    # print("Initial weights after input layer:")
    # print(w1)

    #print('w2 shape')
    w2 = 2 * np.random.random((num_of_hidden, k)) - 1
    #print(w2.shape)
    # print("Initial weights after hidden layer:")
    # print(w2)

    for j in range(num_of_iter):
        l1 = np.matmul(x_new, w1)

        #print('l1 shape before act')
        #print(l1.shape)
        l1 = act_funct(l1)  # output of the hidden layer after activation
        #print('l1 shape after act')
        #print(l1.shape)

        #print(l1[0])


        l2 = np.matmul(l1, w2)
        #print('l2 shape before act')
        #print(l2.shape)
        l2 = softmax_act_funct(l2)  # final output after activation
        #print('l2 shape after act')
        #print(l2.shape)


        #print('values of y_new and corresponding l2')
        #print(y_new[0])
        #print(y_new[1])

        epsilon = 1e-9
        if(j%100==0):
            loss =np.sum( - (y_new * np.log(l2+epsilon))  )
            print("Loss after " + str(j) + " iterations: ", loss)
            print(y_new[0])
            print(l2[0])
            #print(sum(l2[0]))

        delta2 = l2-y_new  # element-wise multiplication
        #print('delta_shape')
        #print(delta2.shape)


        # updating weights for output layer
        w2 = w2 - learning_rate * np.matmul(l1.T, delta2)

        a = np.matmul(delta2, w2.T)
        l1 = slope_act_funct(l1)
        delta1 = a * l1

        # updating weights for hidden layer
        w1 = w1 - learning_rate * np.matmul(x_new.T, delta1)

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

w1,w2,l2=train(x_new,y_new,num_of_hidden,num_of_iter,target_error)


results(l2,y_new)