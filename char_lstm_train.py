from __future__ import absolute_import, division, print_function

import os, sys, argparse
import urllib

import tflearn
from tflearn.data_utils import *

import pickle

parser = argparse.ArgumentParser(description=
    'Pass a text file to generate LSTM output')

parser.add_argument('filename')
parser.add_argument('-t','--temp', help=
    'Defaults to displaying multiple temperature outputs which is suggested.' +
    ' If temp is specified, a value of 0.0 to 2.0 is recommended.' +
    ' Temperature is the novelty or' +
    ' riskiness of the generated output.  A value closer to 0 will result' +
    ' in output closer to the input, so higher is riskier.', 
    required=False, nargs=1, type=float)
parser.add_argument('-l','--length', help=
    'Optional length of text sequences to analyze.  Defaults to 25.',
    required=False, default=25, nargs=1, type=int)
parser.add_argument('-g', '--generate', help=
    'Optional length of text to generate at the end of each epoch. Defaults to 600.',
    required=False, default=600, nargs=1, type=int)
parser.add_argument('-e', '--epochs', help=
    'Number of epochs to train. Default is 50.',
    required=False, default=50, nargs=1, type=int)

args = vars(parser.parse_args())

path = args['filename']
if args['temp'] and args['temp'][0] is not None:
    temp = args['temp'][0]
    print("Temperature set to", temp)
    if temp > 2 or temp < 0:
        print("Temperature out of suggested range.  Suggested temp range is 0.0-2.0") 
    else:
        print("Will display multiple temperature outputs")

if args['length'] is not 25: 
    maxlen = args['length'][0] # default 25 is set in .add_argument above if not set by user
    print("Sequence max length set to ", maxlen)
else:
    maxlen = args['length']

if args['generate'] is not 600: 
    genlen = args['generate'][0] # default 600 is set in .add_argument above if not set by user
    print("Generate length set to ", genlen)
else:
    genlen = args['generate']

if args['epochs'] is not 50: 
    epochs = args['epochs'][0] # default 50 is set in .add_argument above if not set by user
    print("Epochs set to ", epochs)
else:
    epochs = args['epochs']


model_name=path.split('.')[0]  # create model name from textfile input

if not os.path.isfile(path):
    print("Couldn't find the text file. Are you sure the you passed is correct?")

X, Y, char_idx = \
    textfile_to_semi_redundant_sequences(path, seq_maxlen=maxlen, redun_step=3)

g = tflearn.input_data([None, maxlen, len(char_idx)])
g = tflearn.lstm(g, 512, return_seq=True)
g = tflearn.dropout(g, 0.5)
g = tflearn.lstm(g, 512, return_seq=True)
g = tflearn.dropout(g, 0.5)
g = tflearn.lstm(g, 512)
g = tflearn.dropout(g, 0.5)
g = tflearn.fully_connected(g, len(char_idx), activation='softmax')
g = tflearn.regression(g, optimizer='adam', loss='categorical_crossentropy',
                       learning_rate=0.001)

m = tflearn.SequenceGenerator(g, dictionary=char_idx,
                              seq_maxlen=maxlen,
                              clip_gradients=5.0,
                              checkpoint_path='model_'+ model_name)

with open(model_name + '_dict.pkl', 'wb') as dictfile:
    pickle.dump(char_idx, dictfile)
    
for i in range(epochs):
    seed = random_sequence_from_textfile(path, maxlen)
    m.fit(X, Y, validation_set=0.1, batch_size=128,
          n_epoch=1, run_id=model_name)
    print("-- TESTING...")
    if args['temp'] is not None:
        temp = args['temp'][0]
        print("-- Test with temperature of %s --" % temp)
        print(m.generate(genlen, temperature=temp, seq_seed=seed))
    else:
        print("-- Test with temperature of 2.0 --")
        print(m.generate(genlen, temperature=2.0, seq_seed=seed))
        print("-- Test with temperature of 1.75 --")
        print(m.generate(genlen, temperature=1.75, seq_seed=seed))
        print("-- Test with temperature of 1.5 --")
        print(m.generate(genlen, temperature=1.5, seq_seed=seed))
        print("-- Test with temperature of 1.25 --")
        print(m.generate(genlen, temperature=1.25, seq_seed=seed))
        print("-- Test with temperature of 1.0 --")
        print(m.generate(genlen, temperature=1.0, seq_seed=seed))
        print("-- Test with temperature of 0.75 --")
        print(m.generate(genlen, temperature=0.75, seq_seed=seed))
        print("-- Test with temperature of 0.5 --")
        print(m.generate(genlen, temperature=0.5, seq_seed=seed))
        print("-- Test with temperature of 0.25 --")
        print(m.generate(genlen, temperature=0.25, seq_seed=seed))

    m.save(model_name + '_e'+ str(i) + '.model')
