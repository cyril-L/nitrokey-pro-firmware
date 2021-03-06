GTEST = /usr/src/gtest
FIRMWARE_SRC = ../..

INCDIRS = . \
	$(GTEST) \
	$(FIRMWARE_SRC) \
	$(FIRMWARE_SRC)/inc \
	$(FIRMWARE_SRC)/stm/Libraries/STM32_USB-FS-Device_Driver/inc

CFLAGS = -g -Wall -Wextra -lpthread -pthread

CFLAGS += $(patsubst %,-I%,$(INCDIRS))

.PHONY: all
all: tests

.PHONY: clean
clean:
	rm -f *.o tests

.PHONY: run_tests
run_tests: tests
	./tests

%.o : $(GTEST)/src/%.cc
	g++ $(CFLAGS) -c $< -o $@

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

USB_DESCRIPTORS_TEST = usb_descriptors_test.o CCIDHID_usb_desc.o

CCIDHID_usb_desc.o: $(FIRMWARE_SRC)/ccid/CCIDHID_USB/CCIDHID_usb_desc.c
	gcc $(CFLAGS) -c $< -o $@

KEYBOARD_EVENT_QUEUE_TEST = keyboard_event_queue.o hid_keyboard.o

hid_keyboard.o: $(FIRMWARE_SRC)/keybi/hid_keyboard.c
	gcc $(CFLAGS) -c $< -o $@

KEYMAP_TEST = keymap_test.o keymap.o

keymap.o: $(FIRMWARE_SRC)/keybi/keymap.c
	gcc $(CFLAGS) -c $< -o $@

tests: gtest-all.o gtest_main.o $(USB_DESCRIPTORS_TEST) $(KEYBOARD_EVENT_QUEUE_TEST) $(KEYMAP_TEST)
	g++ $(CFLAGS) $^ -o $@