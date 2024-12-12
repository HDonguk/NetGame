//#include "Common.h"
#include "GameThread.h"
#include <iostream>      // �ܼ� �����
#include <chrono>        // ������ �� �ð� ����
#include <thread>        // sleep_for ���
#include "stdafx.h"
#include  "list"
#include "Client.h"
#include<vector>
std::vector<PlayerStatusPacket> PlayerStatus;  // ���� ��Ŷ ����Ʈ ����
std::vector<c_playerPacket> receivedPlayerPackets;
std::vector<c_bulletPacket> receivedBulletPackets;
std::vector<s_playerPacket> sendPlayerPackets;
std::vector<s_bulletPacket> sendBulletPackets;

vector<s_enemyPacket> enemies = {};
vector<s_obstaclePacket> obstacles = {};
vector<s_bulletPacket> Sendbullets = {};
vector<s_itemPacket> items = {};
vector<s_playerPacket> sendPlayers = {};
extern CRITICAL_SECTION cs;         // Critical Section ���� ���� ����
// 30FPS ����
#define FRAME_TIME 0.033f

GameThread::GameThread() {
    // �⺻ ������ ���� (�ʿ信 ���� �ʱ�ȭ �۾� �߰�)
    serverSocket = INVALID_SOCKET;
    std::cout << "�⺻ GameThread ������ ȣ��" << std::endl;
}

GameThread::GameThread(SOCKET serverSocket)
    : serverSocket(serverSocket), gameRunning(false) {}

GameThread::~GameThread() {
    stop();
}


void GameThread::run() {
    gameRunning = true;
    cout << "gameThread run()" << endl;
    while (gameRunning) {
        auto frameStartTime = std::chrono::steady_clock::now();

    //1. updateEnemys
    //2. updateBullet
    //3. collisionAction
       //cout << "updatePlayerStatus()" << endl;
       updatePlayerStatus();
       //updateBulletStatus();
       Sleep(32);
   
        waitUntilNextFrame(frameStartTime);
    }
}

void GameThread::updatePlayerStatus() {
    EnterCriticalSection(&cs); // ����ȭ
    for (const auto& c_player : receivedPlayerPackets) {
        s_playerPacket s_player = {};
        s_player.s_playerID = c_player.c_playerID;
        s_player.s_playerPosX = c_player.c_playerPosX;
        s_player.s_playerPosY = c_player.c_playerPosY;

        
        sendPlayers.push_back(s_player); // ���� ���Ϳ� �߰�
        std::cout 
            << "[LOG(GameTread)] ID=" << s_player.s_playerID
            << ", PosX=" << s_player.s_playerPosX
            << ", PosY=" << s_player.s_playerPosY << std::endl;
        
    }
    receivedPlayerPackets.clear(); // ó���� ������
    LeaveCriticalSection(&cs); // ����ȭ ����
}

void GameThread::updateBulletStatus()
{
    EnterCriticalSection(&cs); // ����ȭ
    for (const auto& c_bullet : receivedBulletPackets) {
        s_bulletPacket s_bullet = {};
        s_bullet.s_playerX = c_bullet.c_playerX;
        s_bullet.s_playerY = c_bullet.c_playerY;
        s_bullet.s_targetX = c_bullet.c_targetX;
        s_bullet.s_targetY = c_bullet.c_targetY;

        // ������Ʈ�� s_bulletPacket�� bullets ���Ϳ� �߰�
        Sendbullets.push_back(s_bullet);

        std::cout << "[LOG(GameThread)] Bullet Packet Received: PlayerX=" << s_bullet.s_playerX
            << ", PlayerY=" << s_bullet.s_playerY
            << ", DirX=" << s_bullet.s_targetX
            << ", DirY=" << s_bullet.s_targetY << std::endl;
    }
    receivedBulletPackets.clear(); // ó���� ������ ����
    LeaveCriticalSection(&cs); // ����ȭ ����
}




void GameThread::waitUntilNextFrame(const std::chrono::time_point<std::chrono::steady_clock>& frameStartTime) {
    auto frameEndTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = frameEndTime - frameStartTime;

    if (elapsed.count() < FRAME_TIME) {
        std::this_thread::sleep_for(std::chrono::duration<float>(FRAME_TIME - elapsed.count()));
    }
}

void GameThread::stop() {
    gameRunning = false;
}

vector<PlayerStatusPacket> GameThread::makeSendPlayerPacket()
{
    // �ۼ� �ʿ�
    return vector<PlayerStatusPacket>();
}



// s_playerPacket�� ������Ʈ
//vector<s_playerPacket> GameThread::makePlayerPacket()
//{
//    s_playerPacket packet = {};
//    for (const Player& p : players)
//    {
//        packet.s_playerID = p.GetID();
//        packet.s_playerPosX = p.GetX();
//        packet.s_playerPosY = p.GetY();
//        packet.s_playerSpeed = p.speed;
//        packet.s_playerHealth = p.GetHealth();
//        packet.s_playerLevel = p.level;
//        packet.s_playerEXP = p.experience;
//    }
//    return vector<s_playerPacket>();
//}