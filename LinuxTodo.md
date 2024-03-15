## Main focus
[] compile and run on linux.  
[] fix bugs that are going to occur  
[] try to reenable as much as code as I can  
[] make this version run on win again (to create a PR)

### disabled stuff
- **netcode**: seems not really important at the first glance, it seems it will be an easy fix to make it platform independent but if I am there and fixing it, I will might as well try to separate it to it's own folder (this seems to be a bigger problem) 

- **random asm blocks**: (why?) I will try to create C code from them if possible but there are blocks that contains thousands (!) of lines of asm code..

- **fmvids**: imo it is a keeperfx change, might the intro showing functionality use it as well (but I can live without that)

- **sndlib**: sound handling.. it loads WSND7R.dll and this seems a bigger issue, I will test out [this](https://github.com/taviso/loadlibrary/blob/master/peloader/winnt_types.h) if it makes it possible to use it as is.


### fixed stuff
- `_findfirst, _findnext, _findclose`: heavy Win dependency but it's just a file iterator. Made my own in the `platform` folder.  
(Lets not mention that I even discovered a memory leak with one of the functions using them AND the fact `stuct TbFileFind` had like 13 parameters but only used **3** (where two were just handles)... and even had converters and a bunch of dead code surrounding it.)