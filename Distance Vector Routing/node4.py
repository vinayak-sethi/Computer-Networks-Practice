from Node import Node
from constants import *
from helperFunctions import printDict


def sendRecvRTT():
    n1 = node4.recvRTT("127.0.0.1")
    node4.restart()
    n2 = node4.recvRTT("127.0.0.2")
    node4.restart()
    n3 = node4.recvRTT("127.0.0.3")
    node4.restart()
    node4.sendRTT(node4RT["rtt"])
    node4.restart()
    return n1, n2, n3


def updateRTTList():
    for _ in range(4):
        minList = []
        if node4RT["rtt"][_] != "0":
            minList.append(float(node4RT["rtt"][0]) + float(node1[_]))
            minList.append(float(node4RT["rtt"][1]) + float(node2[_]))
            minList.append(float(node4RT["rtt"][2]) + float(node3[_]))
            node4RT["rtt"][_] = str(min(minList))
            node4RT["next hop"][_] = nextOptions[minList.index(min(minList))]


if __name__ == "__main__":
    node4RT = {
        "rtt": [],
        "next hop": ["node1", "node2", "node3", "node4"]
    }
    nextOptions = ["node1", "node2", "node3"]
    node4 = Node(IP4, PORT)
    node4RT["rtt"].append(node4.client(IP1))
    print("node1, node4: " + node4RT["rtt"][-1])
    node4.restart()
    node4RT["rtt"].append(node4.client(IP2))
    print("node2, node4: " + node4RT["rtt"][-1])
    node4.restart()
    node4RT["rtt"].append(node4.client(IP3))
    print("node3, node4: " + node4RT["rtt"][-1])
    node4.restart()
    node4.server()
    node4RT["rtt"].append("0")
    print("node4, node4: " + node4RT["rtt"][-1])
    node4.restart()
    for i in range(3):
        node4.port += 1
        node1, node2, node3 = sendRecvRTT()
        updateRTTList()
        print("\nIteration {it}: ".format(it=str(i+1)))
        printDict(node4RT)
        node4.restart()
