from pickle import dumps, loads
from socket import *
from time import sleep, time


class Node:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.node = socket(AF_INET, SOCK_STREAM)

    def restart(self):
        self.node.close()
        self.node = socket(AF_INET, SOCK_STREAM)

    def server(self):
        self.node.bind((self.host, self.port))
        self.node.listen(10)
        for z in range(3):
            node1, addr = self.node.accept()
            start = time()
            for i in range(100):
                node1.send(bytes('1', 'utf-8'))
            node1.recv(8).decode('utf-8')
            end = time()
            rtt = str((end - start) / 100)
            node1.send(bytes(rtt, 'utf-8'))
            node1.close()

    def client(self, host):
        sleep(1)
        self.node.connect((host, self.port))
        for i in range(100):
            self.node.recv(1).decode('utf-8')
        self.node.send(bytes('2', 'utf-8'))
        return self.node.recv(1024).decode('utf-8')

    def sendRTT(self, data):
        self.node.bind((self.host, self.port))
        self.node.listen(10)
        for z in range(3):
            node1, addr = self.node.accept()
            node1.send(dumps(data))

    def recvRTT(self, host):
        while True:
            try:
                self.node.connect((host, self.port))
                return loads(self.node.recv(10240))
            except:
                continue
