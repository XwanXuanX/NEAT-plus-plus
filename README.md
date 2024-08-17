# NEAT++

## Useful docker commands

#### Build image and start container
Build image: `docker build -t ubuntu .`
Run image and start Container: `docker run -it --name ubuntu ubuntu /bin/bash`

#### Start container
Start, stop, and restart container: `docker container <start:restart:stop> ubuntu`

#### Open container shell
Open container command-line: `docker exec -it ubuntu /bin/bash`
