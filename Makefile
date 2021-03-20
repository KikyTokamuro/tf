CC = gcc
CFLAGS = -Wall -O2 -lconfig

TARGET = tf
SERVICE = tf.service
TARGET_CONF = tf.cfg

.DEFAULT_GOAL := tf

tf: tf.c
	$(CC) $< $(CFLAGS) -o $@

install:
	sudo cp $(TARGET) /usr/local/bin/
	sudo cp $(TARGET_CONF) /etc/
	sudo cp $(SERVICE) /etc/systemd/system/
	sudo chmod +x /usr/local/bin/$(TARGET)

uninstall:
	sudo rm /usr/local/bin/$(TARGET)
	sudo rm /etc/systemd/system/$(SERVICE)
	sudo rm /etc/$(TARGET_CONF)

clean:
	rm $(TARGET)
