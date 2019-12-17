# florianhofer.it - a Simple, challenging website

The name says it all: a website development repository, yes. Maybe an attempt of me, a PhD student, to show off alternative ways. There are many paths to follow to create a website; probably the hardest is using C. 

In this website implementation I create html output using C coded CGI scripts for apache compatible servers. So far there is no other techique in use than standard C, client-side JS and CSS. **No** external libraries, no cheats. Why do it the easy way if you can have it the hard? For sure, if you can manage this, dig deep into how web servers work, you can also do it the modern - reuse oriented - way.

I will develop this in spare time - it will take YEARS - however, bare with me. Maybe something funny - slash - useful comes out of it. Or maybe it's just a thing for me and helps showing myself that I can do it.. who knows.

## Directories

    .
    ├── src			# Source files
    |   ├── lib			# static library source code
    |   ├── include		# header files for static library
    |   └── site		# CGI script sources
    ├── www			# contains the public site with compiled CSS, JS and CGI files.
    ├── httpd.conf		# configuration for the `httpd` container on [Docker hub](https://hub.docker.com)
    ├── LICENSE
    ├── MAKEFILE
    └── README.md

Other files are configutation and build files for `make` and CDT.

The master branch should more or less reflect what's loaded on the [domain](https://www.florianhofer.it/) at the moment. `develop` is the actual running development stage and can be joyfully tried out by just running your very own Docker-container webserver.
Glad do receive any feedback or suggestions. Just write me at `website *at* florianhofer.it`

Hope you enjoy!

Florian
