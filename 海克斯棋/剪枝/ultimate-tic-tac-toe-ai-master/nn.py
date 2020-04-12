import math

import numpy as np


class NeuralNetwork:
    def __init__(self, input_shape):
        self.input_shape = input_shape
        self.weights = []
        self.layer1_nodes = 16
        self.layer2_nodes = 8
        self.num_nodes = self.input_shape[1] * self.layer1_nodes + self.layer1_nodes * self.layer2_nodes + self.layer2_nodes
        self.weights.append(np.random.rand(input_shape[1], self.layer1_nodes))
        self.weights.append(np.random.rand(self.layer1_nodes, self.layer2_nodes))
        self.weights.append(np.random.rand(self.layer2_nodes, 1))

    def feedforward(self, input):
        layer = np.asarray([input])
        for weight in self.weights:
            dot_product = np.dot(layer, weight)
            layer = relu_v(dot_product)
        return np.asscalar(layer)

    def weights_to_list(self):
        weight_list = []
        for layer_weights in self.weights:
            weight_list.extend(layer_weights.flatten().tolist())
        return weight_list

    def weights_to_ndarray(self, weight_list):
        weight_ndarray = []

        weight_layer1 = weight_list[:self.input_shape[1]*self.layer1_nodes]
        weight_ndarray.append(np.asarray(weight_layer1).reshape(self.input_shape[1], self.layer1_nodes))

        weight_layer2 = weight_list[self.input_shape[1]*self.layer1_nodes:self.input_shape[1]*self.layer1_nodes + self.layer1_nodes*self.layer2_nodes]
        weight_ndarray.append(np.asarray(weight_layer2).reshape(self.layer1_nodes, self.layer2_nodes))

        weight_layer3 = weight_list[self.input_shape[1]*self.layer1_nodes + self.layer1_nodes*self.layer2_nodes:]
        weight_ndarray.append(np.asarray(weight_layer3).reshape(self.layer2_nodes, 1))
        return weight_ndarray


def sigmoid(x):
    return 1 / (1 + math.exp(-x))


def relu(x):
    if x < 0:
        return 0
    elif x >= 0:
        return x


sigmoid_v = np.vectorize(sigmoid)
relu_v = np.vectorize(relu)
