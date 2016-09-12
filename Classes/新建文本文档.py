import os
import os.path
rootdir = "H:\\cocos2d-x-3.9\\tools\\cocos2d-console\\bin\\TestCpp\\Classes"
outfile = "H:\\cocos2d-x-3.9\\tools\\cocos2d-console\\bin\\TestCpp\\Classes\\output.txt"                                 
output_file = open(outfile, 'w')
for parent,dirnames,filenames in os.walk(rootdir):
    for filename in filenames:
        if os.path.splitext(filename)[1] == '.cpp':
            text = os.path.join(parent,filename)
            text2 = text.replace(rootdir,'../../Classes')
            text3 = text2.replace('\\','/')
            output_file.write(text3 + '\\' + '\n')
output_file.close()

