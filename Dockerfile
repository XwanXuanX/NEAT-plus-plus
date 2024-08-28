# Check http://releases.llvm.org/download.html#10.0.0 for the latest available binaries
FROM ubuntu:latest
LABEL Description="Build environment"

ENV HOME /root

# Make sure the image is updated, install some prerequisites,
# Download the latest version of Clang (official binary) for Ubuntu
# Extract the archive and add Clang to the PATH
RUN apt-get update && apt-get install -y \
  cmake \
  graphviz \
  build-essential

# Copy local files to container
COPY . /usr/src/NEAT++/

# Start from a Bash prompt
CMD [ "/bin/bash" ]
