
Fill a 960x960 pixel grid with an envelope signal.

This program depends on the OS Mesa graphics library, for OpenGL line drawing.

```console
     sudo apt-get install libosmesa6-dev
```

The syntax is color, then start time, end time, start signal strength, ending signal strength. The time goes from 0.0..1.0 and the signal strength is -1.0..1.0

```
WHITE, 0.0, 0.20, 0.50, 0.90
WHITE, 0.20, 0.80, 0.90, -0.30 
WHITE, 0.80, 1.0, -0.30, 0.40
```

Put that in a file called signal.dat

Then run:

```console
make signal.png
```

To view the image:

```console
feh --fullscreen signal.png
```

![Image of output](https://phrasep.com/~lvecsey/software/envgd/output_envgd.png)

