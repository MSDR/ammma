#a script that filters a document to only include lines with one 5-letter word, a-z.
#also converts each character to lowercase

r = open("dict.txt", "r")
w = open("dict5.txt", "w")

l = r.readline()
while(l is not None):
    if(len(l) == 6 and l[0:5].isalpha()):
        w.write(l.lower())
        
    l = r.readline()

r.close()
w.close()