# Installing CUDA



## Installing CuDNN

CuDNN is a library for CUDA, developed by NVIDIA, which provides highly tuned implementations of primitives for deep neural networks. CuDNN is said to make deep nets run faster and sometimes using less memory.

Here are the step to make Theano use CuDNN:

Register at NVIDIA cuDNN.
Download cuDNN v5 Library for Windows 10.
Extract the archive and copy the 3 directories ( bin , include and lib ) in C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.5  or  C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0 . If you’re unsure which version of CUDA Theano uses, check your  CUDA_PATH  environment variable.

## 

http://ankivil.com/making-theano-faster-with-cudnn-and-cnmem-on-windows-10/
