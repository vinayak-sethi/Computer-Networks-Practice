from Node import Node
from constants import *
from helperFunctions import printDict


def sendRecvRTT():
    n1 = node3.recvRTT("127.0.0.1")
    node3.restart()
    n2 = node3.recvRTT("127.0.0.2")
    node3.restart()
    node3.sendRTT(node3RT["rtt"])
    node3.restart()
    n4 = node3.recvRTT("127.0.0.4")
    node3.restart()
    return n1, n2, n4


def updateRTTList():
    for _ in range(4):
        minList = []
        if node3RT["rtt"][_] != "0":
            minList.append(float(node3RT["rtt"][0]) + float(node1[_]))
            minList.append(float(node3RT["rtt"][1]) + float(node2[_]))
            minList.append(float(node3RT["rtt"][3]) + float(node4[_]))
            node3RT["rtt"][_] = str(min(minList))
            node3RT["next hop"][_] = nextOptions[minList.index(min(minList))]


if __name__ == "__main__":
    node3RT = {
        "rtt": [],
        "next hop": ["node1", "node2", "node3", "node4"]
    }
    nextOptions = ["node1", "node2", "node4"]
    node3 = Node(IP3, PORT)
    node3RT["rtt"].append(node3.client(IP1))
    print("node1, node3: " + node3RT["rtt"][-1])
    node3.restart()
    node3RT["rtt"].append(node3.client(IP2))
    print("node2, node3: " + node3RT["rtt"][-1])
    node3.restart()
    node3.server()
    node3RT["rtt"].append("0")
    print("node3, node3: " + node3RT["rtt"][-1])
    node3.restart()
    node3RT["rtt"].append(node3.client(IP4))
    print("node4, node3: " + node3RT["rtt"][-1])
    node3.restart()
    for i in range(3):
        node3.port += 1
        node1, node2, node4 = sendRecvRTT()
        updateRTTList()
        print("\nIteration {it}: ".format(it=str(i+1)))
        printDict(node3RT)
        node3.restart()
