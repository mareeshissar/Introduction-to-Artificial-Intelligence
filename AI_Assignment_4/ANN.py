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


class K_Means:
    def __init__(self, k=5, tol=0.001, max_iter=300):
        self.k = k                                      ## number of clusters
        self.tol = tol                                  ## the tol. if the centroids do not move by 0.001, stop
        self.max_iter = max_iter                        ## number of iterations

    def fit(self,data):

        self.centroids = []                             ##initially the centroids dictionary is empty

        for i in range(self.k):
            val = random.choice(data)
            self.centroids.append(val)                 ## initialize the centroids with first K pixels in the image. Can also randomize it as well


        for i in range(self.max_iter):                  ## start to run for max-iterations

            #print('******start of Kmeans*****')
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

            #print('***classifications******')
            #print(self.classifications[2])
            #print(self.classifications)

            #print('***printing self-centroids****')
            prev_centroids = self.centroids[:]
            #print(self.centroids)
            #print(prev_centroids)


            for classification in self.classifications:
                self.centroids[classification] = np.around(np.average(self.classifications[classification],axis=0))

            print(type(self.centroids))


            #print('***after classifications*****')
            #print(self.centroids)

            check_tol = True

            size = len(prev_centroids)
            print(size)
            print(len(self.centroids))
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


X = cv2.imread('scarlet_tanager.jpg',  cv2.IMREAD_COLOR)     ## read the image ( M * N * 3)
print(X.shape)


num_of_row = X.shape[0]
num_of_col = X.shape[1]
k=5

print('row:'  +str(num_of_row))
print('col:'  +str(num_of_col))

img = Image.fromarray(X, 'RGB')
img.show()

X = X.reshape((-1,3))
print('Xshape:' + str(X.shape))

clf = K_Means()
clf.fit(X)

###############generate the true output for future use as well
Y_out = [[0 for i in range(k)] for j in range(num_of_row * num_of_col)]            ## 2-D list
counter = 0

for pixel in X:
    distances = []
    for centroid in clf.centroids:
        distance = math.sqrt( ( pixel[0] - centroid[0])**2 + (pixel[1] - centroid[1])**2 + (pixel[2] - centroid[2])**2 )
        distances.append(distance)
    ind = distances.index(min(distances))
    Y_out[counter][ind] = 1
    pixel[:] = clf.centroids[ind][:]
    counter=counter+1



X = X.reshape((num_of_row,num_of_col,-1))
img2 = Image.fromarray(X, 'RGB')
img2.save('dominante_color.png')
img2.show()

######################################################################################### END OF K MEANS CLUSTERING ###########################################################################################################


original_image = cv2.imread('tiger2.jpeg',  cv2.IMREAD_COLOR)
X_gray_new = [[0 for i in range(num_of_col)] for j in range( num_of_row)]
for x in range(num_of_row):
    for y in range(num_of_col):
        X_gray_new[x][y]=round(original_image[x][y][0]*0.21 + original_image[x][y][1]*0.72 + original_image[x][y][2]*0.07)


X_gray_new = np.array(X_gray_new)
print('X_gray_new:' + str(X_gray_new.shape))


img_gray=Image.open('tiger2.jpeg').convert('L')
img_gray.show()




#define the input
x_inp = [[0 for i in range(9)] for j in range( num_of_row * num_of_col )]
x_inp = np.array(x_inp)
print('x_inp:' + str(x_inp.shape))

###populate the input

pixel_num = 0
for i in range(0,num_of_row):
    for j in range(0, num_of_col):

        if(i-1>=0 and j-1>=0):
            x_inp[pixel_num][0] = X_gray_new[i - 1][j - 1]
        else:
            x_inp[pixel_num][0] = 0

        if(i-1>=0):
            x_inp[pixel_num][1] = X_gray_new[i - 1][j]
        else:
            x_inp[pixel_num][1] = 0

        if(i-1>=0 and j+1<num_of_col):
            x_inp[pixel_num][2]=X_gray_new[i-1][j+1]
        else:
            x_inp[pixel_num][2] = 0

        if(j-1>=0):
            x_inp[ pixel_num][3]=X_gray_new[i][j-1]
        else:
            x_inp[pixel_num][3] = 0

        x_inp[ pixel_num][4]=X_gray_new[i][j]                                #the true center pixel value

        if(j+1<num_of_col):
            x_inp[ pixel_num][5]=X_gray_new[i][j+1]
        else:
            x_inp[pixel_num][5] = 0

        if(i+1<num_of_row and j-1>=0):
            x_inp[ pixel_num][6]=X_gray_new[i+1][j-1]
        else:
            x_inp[pixel_num][6] = 0

        if(i+1<num_of_row):
            x_inp[ pixel_num][7]=X_gray_new[i+1][j]
        else:
            x_inp[pixel_num][7] = 0

        if(i+1<num_of_row and j+1<num_of_col):
            x_inp[pixel_num][8]=X_gray_new[i+1][j+1]
        else:
            x_inp[pixel_num][8] = 0

        pixel_num = pixel_num + 1


x_inp = x_inp / 255.            #flatten the training set

Y_out = np.array(Y_out)
print('shape of Y_out:' + str(Y_out.shape))


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

learning_rate=10e-6
num_of_hidden=60
num_of_iter=1
target_error = 0.1

w1,w2,l2=train(x_inp,Y_out,num_of_hidden,num_of_iter,target_error)


results(l2,Y_out)




def predict(w1,w2,x_test):

    l1 = np.matmul(x_test, w1)
    l1 = act_funct(l1)  # output of the hidden layer after activation

    l2 = np.matmul(l1, w2)

    l2 = softmax_act_funct(l2)  # final output after activation



    return l2


original_image = cv2.imread('scarlet_tanager_test.jpeg',  cv2.IMREAD_COLOR)

num_of_row = original_image.shape[0]
num_of_col = original_image.shape[1]
X_gray_test = [[0 for i in range(num_of_col)] for j in range( num_of_row)]
for x in range(num_of_row):
    for y in range(num_of_col):
        X_gray_test[x][y]=round(original_image[x][y][0]*0.21 + original_image[x][y][1]*0.72 + original_image[x][y][2]*0.07)

#define the input
x_inp_test = [[0 for i in range(9)] for j in range( num_of_row * num_of_col )]
x_inp_test = np.array(x_inp_test)
print('x_inp:' + str(x_inp_test.shape))

###populate the input

pixel_num = 0
for i in range(0,num_of_row):
    for j in range(0, num_of_col):

        if(i-1>=0 and j-1>=0):
            x_inp_test[pixel_num][0] = X_gray_new[i - 1][j - 1]
        else:
            x_inp_test[pixel_num][0] = 0

        if(i-1>=0):
            x_inp_test[pixel_num][1] = X_gray_new[i - 1][j]
        else:
            x_inp_test[pixel_num][1] = 0

        if(i-1>=0 and j+1<num_of_col):
            x_inp_test[pixel_num][2]=X_gray_new[i-1][j+1]
        else:
            x_inp_test[pixel_num][2] = 0

        if(j-1>=0):
            x_inp_test[ pixel_num][3]=X_gray_new[i][j-1]
        else:
            x_inp_test[pixel_num][3] = 0

        x_inp_test[ pixel_num][4]=X_gray_new[i][j]                                #the true center pixel value

        if(j+1<num_of_col):
            x_inp_test[ pixel_num][5]=X_gray_new[i][j+1]
        else:
            x_inp_test[pixel_num][5] = 0

        if(i+1<num_of_row and j-1>=0):
            x_inp_test[ pixel_num][6]=X_gray_new[i+1][j-1]
        else:
            x_inp_test[pixel_num][6] = 0

        if(i+1<num_of_row):
            x_inp_test[ pixel_num][7]=X_gray_new[i+1][j]
        else:
            x_inp_test[pixel_num][7] = 0

        if(i+1<num_of_row and j+1<num_of_col):
            x_inp_test[pixel_num][8]=X_gray_new[i+1][j+1]
        else:
            x_inp_test[pixel_num][8] = 0

        pixel_num = pixel_num + 1


x_inp_test = x_inp_test / 255.            #flatten the training set



X_gray_test = np.array(X_gray_test)
print('X_gray_new:' + str(X_gray_test.shape))


#l2 = predict(w1,w2,x_inp_test)

Y_test = [[0 for i in range(3)] for j in range( num_of_row * num_of_col )]
Y_test = np.array(Y_test)

print('Y_Test shape' + str(Y_test.shape))

print('clf.centroids len:')
print(len(clf.centroids))
print(clf.centroids[1])
for i in range(0,( num_of_row * num_of_col )):
    max = l2[i][0]
    index_new = 0
    for j in range(0 , k):
        if max < l2[i][j]:
            index_new  = j
    Y_test[i][:] = clf.centroids[index_new][:]

print('print' + str(Y_test[1]))

Y_test = np.array(Y_test)
Y_test = Y_test.reshape((num_of_row,num_of_col,-1))

print('results:')
img_test= Image.fromarray(Y_test, 'RGB')
img_test.save('test.png')
img_test.show()
