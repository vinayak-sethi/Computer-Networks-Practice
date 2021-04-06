from Node import Node
from constants import *
from helperFunctions import printDict


def sendRecvRTT():
    node1.sendRTT(node1RT["rtt"])
    node1.restart()
    n2 = node1.recvRTT("127.0.0.2")
    node1.restart()
    n3 = node1.recvRTT("127.0.0.3")
    node1.restart()
    n4 = node1.recvRTT("127.0.0.4")
    node1.restart()
    return n2, n3, n4


def updateRTTList():
    for _ in range(4):
        minList = []
        if node1RT["rtt"][_] != "0":
            minList.append(float(node1RT["rtt"][1]) + float(node2[_]))
            minList.append(float(node1RT["rtt"][2]) + float(node3[_]))
            minList.append(float(node1RT["rtt"][3]) + float(node4[_]))
            node1RT["rtt"][_] = str(min(minList))
            node1RT["next hop"][_] = nextOptions[minList.index(min(minList))]


if __name__ == "__main__":
    node1RT = {
        "rtt": [],
        "next hop": ["node1", "node2", "node3", "node4"]
    }
    nextOptions = ["node2", "node3", "node4"]
    node1 = Node(IP1, PORT)
    node1.server()
    node1RT["rtt"].append("0")
    print("node1, node1: " + node1RT["rtt"][-1])
    node1.restart()
    node1RT["rtt"].append(node1.client(IP2))
    print("node2, node1: " + node1RT["rtt"][-1])
    node1.restart()
    node1RT["rtt"].append(node1.client(IP3))
    print("node3, node1: " + node1RT["rtt"][-1])
    node1.restart()
    node1RT["rtt"].append(node1.client(IP4))
    print("node4, node1: " + node1RT["rtt"][-1])
    node1.restart()
    for i in range(3):
        node1.port += 1
        node2, node3, node4 = sendRecvRTT()
        updateRTTList()
        print("\nIteration {it}: ".format(it=str(i+1)))
        printDict(node1RT)
        node1.restart()
