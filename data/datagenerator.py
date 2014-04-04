##generate 32 lines of binery data

import random

f = open("data.txt", 'w');
for x in xrange(0,32):
    for y in xrange(0,32):
        s = random.uniform(0,1);
        c = int(round(s-0.4));  
        f.write(str(c));
    f.write('\n');
f.close();


