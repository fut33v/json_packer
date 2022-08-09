FROM debian:latest

RUN mkdir -p /home/app
ENV HOME=/home/app
WORKDIR $HOME

RUN apt-get update
RUN apt-get install -y gcc g++ cmake

COPY . json_packer
WORKDIR $HOME/json_packer
RUN rm -rf build

CMD ["/home/app/json_packer/run.sh"]
