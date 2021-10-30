#pragma once

#include <inttypes.h>
#include <vector>
#include <memory>

typedef uint8_t CaissaPieceIdentifier;
#define CAISSA_PIECEID_INVALID 0xFF; // 1111 1111

typedef uint8_t CaissaTeamIdentifier;
#define CAISSA_TEAMID_INVALID 0xFF; // 1111 1111

struct CaissaMove
{
public:
    uint16_t OriginRank, OriginFile;
    uint16_t TargetRank, TargetFile;

public:
    CaissaMove(uint16_t originRank, uint16_t originFile, uint16_t targetRank, uint16_t targetFile)
        : OriginRank(originRank), OriginFile(originFile), TargetRank(targetRank), TargetFile(targetFile) {}
};

typedef struct CaissaConfiguration
{
public:
    uint16_t RankCount;
    uint16_t FileCount;
};