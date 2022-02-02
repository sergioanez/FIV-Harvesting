# FIV force prediction neural network with 30 evenly spaced sensors (5x6)


# sensor locations:

# evenly spaced 6x5 grid
import mat73 as mat73

x_sensors = 6
y_sensors = 5
num_sensors = x_sensors * y_sensors

### Importing reduced data
# we pre selected data at 30 sensor locations(5x6 grid) in MATLAB

pip install mat73

import scipy.io as spio
import mat73

from google.colab import drive

drive.mount('/content/gdrive')

!unzip gdrive / My\ Drive / Colab\ Notebooks / FIV\ force\ prediction / reduced_files.zip

### putting data in dataframe

import pandas as pd
import numpy as np
import itertools
import seaborn as sns
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import IPython
import IPython.display
from scipy import interpolate
from scipy.interpolate import interp1d
from sklearn import linear_model

# Make NumPy printouts easier to read.
np.set_printoptions(precision=3, suppress=True)

uStars = ["uStar3_75", "uStar4_50", "uStar5_50", "uStar5_75",
          "uStar6_50", "uStar7_50", "uStar8_00", "uStar8_25",
          "uStar8_75", "uStar9_00", "uStar9_25", "uStar10_50"]

dataset = pd.DataFrame()

for uStar in uStars:
    # organize data
    posVel = mat73.loadmat(uStar + '/positionVelocity.mat')
    reducedField_5_6 = spio.loadmat(uStar + '/reducedField_5_6.mat', squeeze_me=True)
    reducedField_5_6 = reducedField_5_6['reducedField']
    config = spio.loadmat(uStar + '/configuration.mat', squeeze_me=True)

    xIndices = np.linspace(0, config['numX'] - 1, x_sensors).astype(int)
    yIndices = np.linspace(0, config['numY'] - 1, y_sensors).astype(int)
    x = config["X"][0, xIndices]
    y = config["Y"][yIndices, 0]

    numT = config['numT']
    time_steps = np.arange(numT)

    # add x, y, time steps
    a = [y, x, time_steps]
    data = list(itertools.product(*a))
    df = pd.DataFrame(data=data, columns=["y", "x", "t"])

    # add u and v
    df['u'] = reducedField_5_6[:, :, :, 0].flatten()
    df['v'] = reducedField_5_6[:, :, :, 1].flatten()

    # add cylinder properties (lift, drag, pos, vel)
    df['lift'] = np.tile(posVel['lift'], x_sensors * y_sensors)
    df['drag'] = np.tile(posVel['drag'], x_sensors * y_sensors)
    df['pos'] = np.tile(posVel['position'], x_sensors * y_sensors)
    df['vel'] = np.tile(posVel['velocity'], x_sensors * y_sensors)

    # add uStar
    uStar_float = uStar[5:]
    uStar_float = uStar_float.replace('_', '.')
    uStar_float = float(uStar_float)
    df['uStar'] = uStar_float

    dataset = dataset.append(df)

dataset.reset_index(inplace=True, drop=True)

dataset

### Looking at specific uStar

uStar = 7.50

dataset_10_50 = dataset[dataset['uStar'] == uStar]
dataset_10_50 = dataset_10_50.sort_values(by=['t'])
dataset_10_50 = dataset_10_50.drop(columns=['y', 'x', 'pos', 'vel', 'uStar'])

# looking at the data
dataset_10_50.describe().transpose()

# reshaping

dataset_10_50['idx'] = dataset_10_50.groupby('t').cumcount()
dataset_10_50.pivot(index='t', columns='idx')[['u', 'v']]

dataset_10_50['u_idx'] = 'u_' + dataset_10_50.idx.astype(str)
dataset_10_50['v_idx'] = 'v_' + dataset_10_50.idx.astype(str)

u = dataset_10_50.pivot(index='t', columns='u_idx', values='u')
v = dataset_10_50.pivot(index='t', columns='v_idx', values='v')

dataset_10_50_wide = pd.concat([u, v], axis=1)
dataset_10_50_wide['lift'] = dataset_10_50.set_index('t')['lift'].drop_duplicates()
dataset_10_50_wide['drag'] = dataset_10_50.set_index('t')['drag'].drop_duplicates()
dataset_10_50_wide

# rename df
df = dataset_10_50_wide.copy()

# DROP LIFT TEMPORARILY
df = df.drop(columns=['lift'])

# split the data
column_indices = {name: i for i, name in enumerate(df.columns)}

n = len(df)
train_df = df[0:int(n * 0.5)]
val_df = df[int(n * 0.5):int(n * 0.9)]
test_df = df[int(n * 0.9):]

num_features = df.shape[1]

# normalize the data
train_mean = train_df.mean()
train_std = train_df.std()

train_df = (train_df - train_mean) / train_std
val_df = (val_df - train_mean) / train_std
test_df = (test_df - train_mean) / train_std

# looking at the normalization:
df_std = (df - train_mean) / train_std
df_std = df_std.melt(var_name='Column', value_name='Normalized')
plt.figure(figsize=(12, 6))
ax = sns.violinplot(x='Column', y='Normalized', data=df_std)
_ = ax.set_xticklabels(df.keys(), rotation=90)


# Data windowing

# defining prediction window

class WindowGenerator():
    def __init__(self, input_width, label_width, shift,
                 train_df=train_df, val_df=val_df, test_df=test_df,
                 label_columns=None):
        # Store the raw data.
        self.train_df = train_df
        self.val_df = val_df
        self.test_df = test_df

        # all column indices
        self.column_indices = {name: i for i, name in
                               enumerate(train_df.columns)}

        # Work out the label column indices.
        self.label_columns = label_columns
        if label_columns is not None:
            self.label_columns_indices = {name: i for i, name in
                                          enumerate(label_columns)}

        # Work out the non-label column indices.
        self.all_columns = list(train_df.columns)
        self.non_label_columns = [x for x in self.all_columns if x not in label_columns]
        if self.non_label_columns is not None:
            self.non_label_columns_indices = {name: i for i, name in
                                              enumerate(self.non_label_columns)}

            # Work out the window parameters.
        self.input_width = input_width
        self.label_width = label_width
        self.shift = shift

        self.total_window_size = input_width + shift

        self.input_slice = slice(0, input_width)
        self.input_indices = np.arange(self.total_window_size)[self.input_slice]

        self.label_start = self.total_window_size - self.label_width
        self.labels_slice = slice(self.label_start, None)
        self.label_indices = np.arange(self.total_window_size)[self.labels_slice]

    def __repr__(self):
        return '\n'.join([
            f'Total window size: {self.total_window_size}',
            f'Input indices: {self.input_indices}',
            f'Label indices: {self.label_indices}',
            f'Label column name(s): {self.label_columns}'])


# splitting window into a window of inputs and a window of labels

def split_window(self, features):
    inputs = features[:, self.input_slice, :]
    if self.label_columns is not None:
        inputs = tf.stack(
            [inputs[:, :, self.column_indices[name]] for name in self.non_label_columns],
            axis=-1)

    labels = features[:, self.labels_slice, :]
    if self.label_columns is not None:
        labels = tf.stack(
            [labels[:, :, self.column_indices[name]] for name in self.label_columns],
            axis=-1)

    # Slicing doesn't preserve static shape information, so set the shapes
    # manually. This way the `tf.data.Datasets` are easier to inspect.
    inputs.set_shape([None, self.input_width, None])
    labels.set_shape([None, self.label_width, None])

    return inputs, labels


WindowGenerator.split_window = split_window


# creating TensorFlow datasets

def make_dataset(self, data):
    data = np.array(data, dtype=np.float32)
    ds = tf.keras.preprocessing.timeseries_dataset_from_array(
        data=data,
        targets=None,
        sequence_length=self.total_window_size,
        sequence_stride=1,
        shuffle=False,
        batch_size=48, )

    ds = ds.map(self.split_window)

    return ds


WindowGenerator.make_dataset = make_dataset


# Add properties for accessing tf.data.Datasets. Also, add a standard example batches for easy access and plotting:

@property
def train(self):
    return self.make_dataset(self.train_df)


@property
def val(self):
    return self.make_dataset(self.val_df)


@property
def test(self):
    return self.make_dataset(self.test_df)


@property
def example(self):
    """Get and cache an example batch of `inputs, labels` for plotting."""
    result = getattr(self, '_example', None)
    if result is None:
        # No example batch was found, so get one from the `train` dataset
        result = next(iter(self.train))
        # And cache it for next time
        self._example = result
    return result


@property
def val_example(self):
    """Get and cache an example batch of `inputs, labels` for plotting."""
    result = getattr(self, '_val_example', None)
    if result is None:
        # No example batch was found, so get one from the `validation` dataset
        result = next(iter(self.val))
        # And cache it for next time
        self._val_example = result
    return result


@property
def test_example(self):
    """Get and cache an example batch of `inputs, labels` for plotting."""
    result = getattr(self, '_test_example', None)
    if result is None:
        # No example batch was found, so get one from the `test` dataset
        result = next(iter(self.test))
        # And cache it for next time
        self._test_example = result
    return result


WindowGenerator.train = train
WindowGenerator.val = val
WindowGenerator.test = test
WindowGenerator.example = example
WindowGenerator.val_example = val_example
WindowGenerator.test_example = test_example


# LSTM model

# Choosing our paramaters for our network

def compile_and_fit(model, window, patience=3):
    early_stopping = tf.keras.callbacks.EarlyStopping(monitor='val_loss',
                                                      patience=patience,
                                                      mode='min')
    # learning rate scheduler
    # def scheduler(epoch, lr, val_loss):
    #  if epoch < 5:
    #    return lr
    #  else:
    #    return lr * tf.math.exp(-0.1)
    # lr_schedule = tf.keras.callbacks.LearningRateScheduler(scheduler)

    # initial_learning_rate = 0.1
    # lr_schedule = tf.keras.optimizers.schedules.ExponentialDecay(
    #  initial_learning_rate,
    #  decay_steps=100000,
    #  decay_rate=0.96,
    #  staircase=True)

    model.compile(loss=tf.losses.MeanSquaredError(),
                  optimizer=tf.optimizers.Adam(learning_rate=0.0001),
                  metrics=[tf.metrics.MeanAbsoluteError(), tf.metrics.RootMeanSquaredError()])

    history = model.fit(window.train, epochs=MAX_EPOCHS,
                        validation_data=window.val)
    # callbacks=[early_stopping])
    return history


wide_window = WindowGenerator(
    input_width=60, label_width=60, shift=1,
    label_columns=['drag'])

wide_window

MAX_EPOCHS = 100
neurons_per_layer = 128

lstm_model = tf.keras.models.Sequential([
    # Shape [batch, time, features] => [batch, time, lstm_units]
    tf.keras.layers.LSTM(neurons_per_layer, activation='relu', return_sequences=True),
    tf.keras.layers.Dense(units=4, activation='relu'),
    tf.keras.layers.Dropout(0.05, seed=None),
    tf.keras.layers.LSTM(neurons_per_layer, activation='relu', return_sequences=True),
    # Shape => [batch, time, features]
    tf.keras.layers.Dense(units=1)
])

print('Input shape:', wide_window.example[0].shape)
print('Output shape:', lstm_model(wide_window.example[0]).shape)

history = compile_and_fit(lstm_model, wide_window)


def plot_over_time(self, model=None, plot_col=None, example_type=None):
    '''
    plot_col: 'lift' or 'drag'
    example_type:
          'train': training example
          'val': validation example
          'test': test example
    '''
    if example_type == 'train':
        example = self.example
    if example_type == 'val':
        example = self.val_example
    if example_type == 'test':
        example = self.test_example

    inputs, labels = example
    plt.figure(figsize=(12, 8))
    plot_col_index = self.column_indices[plot_col]

    # for spline
    x_label = []
    y_label = []
    x_pred = []
    y_pred = []

    max_n = len(inputs)
    for n in range(max_n):
        plt.ylabel(f'{plot_col} [normed]')
        # plt.plot(self.input_indices, inputs[n, :, plot_col_index],
        # label='Inputs', marker='.', zorder=-10)

        if self.label_columns:
            label_col_index = self.label_columns_indices.get(plot_col, None)
        else:
            label_col_index = plot_col_index

        if label_col_index is None:
            continue

        # changing x axis to seconds
        x_sec = ((self.label_indices + n) + 1) / 24

        # plotting labels
        labels_unnormalized = labels * train_std[label_col_index] + train_mean[label_col_index]
        if n == 0:
            label = 'Labels'
        else:
            label = '_nolegend_'
        plt.scatter(x_sec, labels[n, :, label_col_index],
                    edgecolors='k', label=label, c='#2ca02c', s=64)

        # plotting predictions
        if model is not None:
            predictions = model(inputs)
            predictions_unnormalized = predictions * train_std[label_col_index] + train_mean[label_col_index]
            if n == 0:
                label = 'Predictions'
            else:
                label = '_nolegend_'
            plt.scatter(x_sec, predictions[n, :, label_col_index],
                        marker='X', edgecolors='k', label=label,
                        c='#ff7f0e', s=64)

        # for spline
        x_label.append(float(x_sec))
        y_label.append(float(labels[n, :, label_col_index]))
        x_pred.append(float(x_sec))
        y_pred.append(float(predictions[n, :, label_col_index]))

        if n == 0:
            plt.legend()

    # spline fit
    f_label = interp1d(x_label, y_label, kind='cubic')
    xnew = np.linspace(x_label[0], x_label[-1], num=len(x_label) * 10)
    plt.plot(xnew, f_label(xnew), '-')

    f_pred = interp1d(x_pred, y_pred, kind='cubic')
    plt.plot(xnew, f_pred(xnew), '-')

    plt.title('Ustar = ' + str(uStar) + '\nEpochs: ' + str(MAX_EPOCHS)
              + '\nneurons/layer: ' + str(neurons_per_layer))
    plt.xlabel('Time (s)')


WindowGenerator.plot_over_time = plot_over_time


def plot(self, model=None, plot_col='drag', max_subplots=3):
    inputs, labels = self.example
    plt.figure(figsize=(12, 8))
    plot_col_index = self.column_indices[plot_col]
    max_n = min(max_subplots, len(inputs))
    for n in range(max_n):
        plt.subplot(max_n, 1, n + 1)
        plt.ylabel(f'{plot_col} [normed]')
        plt.plot(self.input_indices, inputs[n, :, plot_col_index],
                 label='Inputs', marker='.', zorder=-10)

        if self.label_columns:
            label_col_index = self.label_columns_indices.get(plot_col, None)
        else:
            label_col_index = plot_col_index

        if label_col_index is None:
            continue

        plt.scatter(self.label_indices, labels[n, :, label_col_index],
                    edgecolors='k', label='Labels', c='#2ca02c', s=64)
        if model is not None:
            predictions = model(inputs)
            plt.scatter(self.label_indices, predictions[n, :, label_col_index],
                        marker='X', edgecolors='k', label='Predictions',
                        c='#ff7f0e', s=64)

        if n == 0:
            plt.legend()

    plt.xlabel('Time [h]')


WindowGenerator.plot = plot

# wide_window.plot(lstm_model)
wide_window.plot_over_time(lstm_model, plot_col='drag', example_type='test')

# Plotting loss vs epochs

val_performance = {}
performance = {}
val_performance['LSTM'] = lstm_model.evaluate(wide_window.val)
performance['LSTM'] = lstm_model.evaluate(wide_window.test, verbose=0)

performance

# Plotting our accuracy and loss functions

mae = history.history['mean_absolute_error']
val_mae = history.history['val_mean_absolute_error']
loss = history.history['loss']
val_loss = history.history['val_loss']
epochs = range(1, len(loss) + 1)

plt.plot(epochs, mae, 'bo--', label='Training acc')
plt.plot(epochs, val_mae, 'ro-', label='Validation acc')
plt.title('Training and validation mean absolute error')
plt.xlabel('Epochs')
plt.ylabel('Mean absolute error')
plt.legend()

plt.figure()

plt.plot(epochs, loss, 'bo--', label='Training loss')
plt.plot(epochs, val_loss, 'ro-', label='Validation loss')
plt.title('Training and validation loss')
plt.xlabel('Epochs')
plt.ylim([0, 1])
plt.ylabel('Loss')
plt.legend()

# Looking at average final loss values

avg_train_loss = np.mean(loss[15:20])  # avg training loss (last 5 epochs)
avg_val_loss = np.mean(val_loss[15:20])  # avg validation loss (last 5 epochs)

print(avg_train_loss)
print(avg_val_loss)
