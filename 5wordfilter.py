
print("running")
r = open("dict.txt", "r")
w = open("dict5.txt", "w")
l = r.readline()
print(l)
while(l is not None):
    if(len(l) == 6 and l[0:5].isalpha()):
        w.write(l.lower())
    l = r.readline()

r.close()
w.close()