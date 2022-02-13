chardev_user_app: chardev_user_app.c

.PHONY: clean
clean:
	-rm -rf chardev_user_app *.o

.PHONY: test
test: chardev_user_app
	-cd ../chardev && $(MAKE) test
	-sudo ./chardev_user_app

