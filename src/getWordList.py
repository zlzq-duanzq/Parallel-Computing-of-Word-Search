# python2 getWordList.py

import random
import urllib2
import ssl

len_limit = 5
num_words = 50

word_site = "http://www.instructables.com/files/orig/FLU/YE8L/H82UHPR8/FLUYE8LH82UHPR8.txt"
ssl._create_default_https_context = ssl._create_unverified_context
response = urllib2.urlopen(word_site)

txt = response.read()
WORDS = txt.splitlines()
WORDS = list(filter(lambda x: len(x) >= len_limit, WORDS))


with open('wordList_50.txt', 'w') as f:
    for i in range(num_words):
        randomword = random.choice(WORDS)
        f.write(randomword)
        f.write('\n')
