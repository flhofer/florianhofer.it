# florianhofer.it - a Simple, challenging website

The name says it all: a website development repository, yes. Maybe an attempt of me, a PhD student, to show off alternative ways. There are many paths to follow to create a website; probably the hardest is using C. 

In this website implementation I create html output using C coded CGI scripts for apache compatible servers. So far there is no other techique in use than standard C, client-side JS and CSS. **No** external libraries, no cheats. Why do it the easy way if you can have it the hard?

I will develop this in spare time - it will take YEARS - however, bare with me. Maybe something funny - slash - useful comes out of it. Or maybe it's just for me and helps showing that I can do it..

## Directories

- www contains the public site with compiled CSS, JS and CGI files. (disregard the php for now, it's not in use)
- src C source 
- *httpd.conf*  configuration for the `httpd` container on [Docker hub](https://hub.docker.com)

Other files are configutation and build files for `make` and CDT.

The master branch should more or less reflect what's loaded on the [domain](https://www.florianhofer.it/) at the moment. `develop` is the actual running development stage and can be joyfully tried out by just running your very own Docker-container webserver.
Glad do receive any feedback or suggestions. Just write me at `website *at* florianhofer.it`

Hope you enjoy!

Florian
