FROM ubuntu
RUN apt update
RUN apt install python3.8 -y \
    python3-pip \
    && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

COPY requirements.txt .
RUN pip3 install -r requirements.txt

COPY static ./APP/static/
COPY templates ./APP/templates/
COPY app.py ./APP/
COPY grapher.py ./APP/

WORKDIR "/APP/"

