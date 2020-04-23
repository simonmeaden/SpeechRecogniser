TEMPLATE = subdirs

SUBDIRS += \
    SpeechRecogniser \
    SpeechRecogniserTest

SpeechRecogniser.subdir = SpeechRecogniser

SpeechRecogniserTest.subdir = SpeechRecogniserTest
SpeechRecogniserTest.depends = SpeechRecogniser
