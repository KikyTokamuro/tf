CC = gcc
CFLAGS = -Wall -O2

TARGET = tf
SERVICE = tf.service

.DEFAULT_GOAL := tf

tf: main.c
	$(CC) $< $(CFLAGS) -o $@

install:
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)
	sudo cp $(SERVICE) /etc/systemd/system/

uninstall:
	sudo rm /usr/local/bin/$(TARGET)
	sudo rm /etc/systemd/system/$(SERVICE)

clean:
	rm $(TARGET)