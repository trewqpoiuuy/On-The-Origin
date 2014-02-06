from subprocess import Popen, PIPE

def copy(text):
    p = Popen(['xsel','-bi'], stdin=PIPE)
    p.communicate(input=text)

def paste():
    p = Popen(['xsel','-b'], stdout=PIPE)
    return p.communicate()[0]
