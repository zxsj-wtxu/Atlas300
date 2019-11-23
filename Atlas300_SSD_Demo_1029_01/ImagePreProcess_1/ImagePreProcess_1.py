import sys
import os
from numpy import *
from PIL import Image
import numpy as np


def convert(para1,para2,para3,para4):
    return (((para4*256+para3)*256+para2)*256)+para1

def readYuvFile(filename):
    fp=open(filename,'rb')
    fp.seek(-32, 2) #the last 32 bytes are info about yuv file which is used to transfer jpg
    resize_width=convert(ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)))
    resize_height=convert(ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)))
    preprocess_width=convert(ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)))
    preprocess_height=convert(ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)))
    frameID=convert(ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)),ord(fp.read(1)))
    reserved=fp.read(12)

    fp.seek(0, 0)
    uv_width=preprocess_width//2
    uv_height=preprocess_height//2

    Y=np.zeros((preprocess_height,preprocess_width),dtype=np.uint8)
    U=np.zeros((uv_height,uv_width),dtype=np.uint8)
    V=np.zeros((uv_height,uv_width),dtype=np.uint8)

    for m in range(preprocess_height):
        for n in range(preprocess_width):
            Y[m,n]=ord(fp.read(1))
    for m in range(uv_height):
        for n in range(uv_width):
            U[m,n]=ord(fp.read(1))
            V[m,n]=ord(fp.read(1))

    fp.close()
    return (Y,U,V),resize_width,resize_height,preprocess_width,preprocess_height,frameID

def yuv2rgb(Y,U,V,width,height):
    U=np.repeat(U,2,0)
    U=np.repeat(U,2,1)
    V=np.repeat(V,2,0)
    V=np.repeat(V,2,1)
    rf=np.zeros((height,width),float,'C')
    gf=np.zeros((height,width),float,'C')
    bf=np.zeros((height,width),float,'C')

    rf=Y+1.403*(V-128.0)
    gf=Y-0.343*(U-128.0)-0.714*(V-128.0)
    bf=Y+1.77*(U-128.0)

    for m in range(height):
        for n in range(width):
            if(rf[m,n]>255):
                rf[m,n]=255
            if(gf[m,n]>255):
                gf[m,n]=255
            if(bf[m,n]>255):
                bf[m,n]=255
            if(rf[m,n]<0):
                rf[m,n]=0
            if(gf[m,n]<0):
                gf[m,n]=0
            if(bf[m,n]<0):
                bf[m,n]=0
    r=rf.astype(np.uint8)
    g=gf.astype(np.uint8)
    b=bf.astype(np.uint8)
    return (r,g,b)

def tranferYUV2JPG(yuvFile):
    data,resize_width,resize_height,preprocess_width,preprocess_height,frameID=readYuvFile(yuvFile)
    imageName=yuvFile.replace('_preprocessYUV', '_preProcess.jpg')

    im_rgb=np.zeros((3, preprocess_height, preprocess_width))
    im_rgb[0], im_rgb[1], im_rgb[2]=yuv2rgb(data[0], data[1], data[2], preprocess_width, preprocess_height)
    im_rgb=np.transpose(im_rgb, (1, 2, 0))

    image=Image.fromarray(im_rgb.astype(np.uint8), mode='RGB')
    image=image.crop((0, 0, resize_width, resize_height))
    image.save(imageName)

if __name__=='__main__':
    param = sys.argv[1]  # raw image path
    yuv_list = param.split(',')
    for yuvFile in yuv_list:
        if "_preprocessYUV" in yuvFile:
            tranferYUV2JPG(yuvFile)
