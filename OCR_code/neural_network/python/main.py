import os
from PIL import Image, ImageOps
import numpy as np


def images_to_file(folder, filename_data, filename_label):
    print("Ready ?",end="")
    input()
    data = open(filename_data, "w")
    label = open(filename_label, "w")
    count = 0
    num = []
    lab = []

    for filename in os.listdir(folder):
        f = os.path.join(folder, filename)
        if os.path.isfile(f) and (f.endswith(".jpg") or f.endswith(".jpeg") or f.endswith(".png")):
            img = Image.open(f, "r")
            img.resize((28, 28)).save(f)
            img = Image.open(f, "r")
            flag = True
            while flag:
                img = ImageOps.grayscale(img)
                tmp = np.array(img)
                for u in range(28):
                    for v in range(28):
                        tmp[u][v] = 255 - tmp[u][v]
                        if tmp[u][v] > 127:
                            print(" ",end="")
                        else:
                            print("#",end="")
                    print()

                print("This image is : ")
                entry = input()
                print("\n")
                if 0 <= int(entry) <= 9:
                    lab.append(int(entry))
                    num.append([])
                    for j in tmp:
                        for val in j:
                            num[count].append(val)
                    count += 1
                    flag = False
                else:
                    flag = False

    # write data
    data.write(str(count) + " 28" + " 28" + " \n")
    for i in num:
        data.write("[")
        for j in range(len(i)):
            if j == len(i) - 1:
                data.write(str(i[j]) + "]\n")
            else:
                data.write(str(i[j]) + ",")

    # write label
    label.write(str(count) + "\n")
    label.write("[")
    for j in range(len(lab)):
        if j == len(lab) - 1:
            label.write(str(lab[j]) + "]\n")
        else:
            label.write(str(lab[j]) + ",")
    data.close()
    label.close()

images_to_file("../../../data/cells","testAnis.test","testAnis.label")