// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>
#include <consensus/consensus.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

///////////////////////////////////////////// // abp
#include <libdevcore/SHA3.h>
#include <libdevcore/RLP.h>
#include "arith_uint256.h"
/////////////////////////////////////////////

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 00 << 488804799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    genesis.hashStateRoot = uint256(h256Touint(dev::h256("e965ffd002cd6ad0e2dc402b8044de833e06b23127ea8c3d80aec91410771495"))); // abp
    genesis.hashUTXORoot = uint256(h256Touint(dev::sha3(dev::rlp("")))); // abp
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Nov 17, 2020 Abp new huns make coin bitcoin up up";
    const CScript genesisOutputScript = CScript() << ParseHex("040d61d8653448c98731ee5fffd303c15e71ec2057b77f11ab3601979728cdaff2d68afbba14e4fa0bc44f2072b0b23ef63717f8cdfbe58dcd33f32b6afe98741a") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 2102400; //abp abp halving every 4 years
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00008435a6d3dd42fd2bc7407d5ff591cfd88d59d3283f50c93ccc1f381dfdfe"); //abp
        consensus.BIP65Height = 0; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 0; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60; //abp final 10 * 12 // 10 * 60; 10 minutes // 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 1 * 60; //abp final [86400s(1day)/10s(10sec) = 8640block] => [8640block * 365day] => [3153600block * 4year] = 12614400block // second 2 * 64; <=//[86400s(1day)/60s(1min) = 1440block] => [1440block * 365day] => [525600block * 4year] = 2102400block // org 2 * 64;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1514764800; //abp //1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1546300799; //abp //1230767999; // December 31, 2008
/*
Epoch timestamp: 1514764800
Timestamp in milliseconds: 1514764800000
Human time (GMT): 2018년 January 1일 Monday AM 12:00:00
Human time (your time zone): 2018년 1월 1일 월요일 오전 9:00:00 GMT+09:00
Epoch timestamp: 1546300799
Timestamp in milliseconds: 1546300799000
Human time (GMT): 2018년 December 31일 Monday PM 11:59:59
Human time (your time zone): 2019년 1월 1일 화요일 오전 8:59:59 GMT+09:00
*/
        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); //abp

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xbfbbfc2c3be3d4e085082aff2e4e73a4e21dbf6205bc41b84b38ffac0a8bc114"); //453354

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf2; //abp
        pchMessageStart[1] = 0xbf; //abp
        pchMessageStart[2] = 0x6b; //abp
        pchMessageStart[3] = 0xc0; //abp
        nDefaultPort = 9981; //abp
        nPruneAfterHeight = 100000;

		//Epoch timestamp: 1546300800
		//Timestamp in milliseconds: 1546300800000
		//Human time (GMT): 2019년 January 1일 Tuesday AM 12:00:00
		//Human time (your time zone): 2019년 1월 1일 화요일 오전 9:00:00 GMT+09:00
        genesis = CreateGenesisBlock(1605588500, 214381, 0x1f00ffff, 1, 50 * COIN); //abp
		consensus.hashGenesisBlock = genesis.GetHash();//uint256 hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x00008435a6d3dd42fd2bc7407d5ff591cfd88d59d3283f50c93ccc1f381dfdfe")); //abp 0x000039c375f664b12c1bd8c7cfc8d3f943c1c5b9411e7f4be1eb4ee2be08d09c
        assert(genesis.hashMerkleRoot == uint256S("0xefd0a530a4567e676430e0a6ea6f3c7c3c2b3873769fc33ee902876634465843")); //abp 0x0ce221cf045c8ed0a9a815dc654d408635a80843f8b560ffb5517a94cdee446f

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they dont support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("abpbc1.fast.kr"); //abp Abpcoin mainnet
        vSeeds.emplace_back("abpbc2.fast.kr"); //abp Abpcoin mainnet
        vSeeds.emplace_back("abpbc3.fast.kr"); //abp Abpcoin mainnet
        vSeeds.emplace_back("abpbc4.fast.kr"); //abp Abpcoin mainnet
        vSeeds.emplace_back("abpbc5.fast.kr"); //abp Abpcoin mainnet


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23); //abp (y) https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,83); //abp
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "ac"; //abp

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                { 0, uint256S("0x00008435a6d3dd42fd2bc7407d5ff591cfd88d59d3283f50c93ccc1f381dfdfe") }, //abp 0x000039c375f664b12c1bd8c7cfc8d3f943c1c5b9411e7f4be1eb4ee2be08d09c
				{ 5000, uint256S("0x000026e781f70287b9519cb5d8e5f9f75295a4aec15983977f53cd40f8f7c5e6") },
				{ 5500, uint256S("0x6007ab7c095ae536c0324756e3b70d04be267f470a435e93bcbecf7f2dda0218") },
				{ 6000, uint256S("0xb2c53112b7abdb6b52f07c152c6ffe582c486ff5fd1ed5edec6da6bb8b77c693") },
				{ 7000, uint256S("0x2b76d4cb419ea040c1c15449b730419f71babcc4e7be7e82f34bc113720be71e") },
				{ 7700, uint256S("0x5da747c6b169b337e528f77bbb01d76ae56e0dbd3ea6ae1b431abe41e9246672") },
			}
        };

        chainTxData = ChainTxData{
            // Data as of block a1bab8db27f26952ce94fff6563931943554e36fc3a23f99cc8513270d685b2c (height 92662)
            0, //abp
            0, //abp
            0  //abp
        };
        consensus.nLastPOWBlock = 5000;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock + 
                                    consensus.nMPoSRewardRecipients + 
                                    COINBASE_MATURITY;

        consensus.nFixUTXOCacheHFHeight=100000;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2102400;// //abp halving every 4 years, second 2102400;  
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00008435a6d3dd42fd2bc7407d5ff591cfd88d59d3283f50c93ccc1f381dfdfe"); //abp 0x000039c375f664b12c1bd8c7cfc8d3f943c1c5b9411e7f4be1eb4ee2be08d09c
        consensus.BIP65Height = 0; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 0; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60; //abp final 10 * 12 //10 * 60; 10 minutes // 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 1 * 60; //abp final [86400s(1day)/10s(10sec) = 8640block] => [8640block * 365day] => [3153600block * 4year] = 12614400block // second 2 * 64; <=//[86400s(1day)/60s(1min) = 1440block] => [1440block * 365day] => [525600block * 4year] = 2102400block // org 2 * 64;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1514764800; //abp //1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1546300799; //abp //1230767999; // December 31, 2008
/*
Epoch timestamp: 1514764800
Timestamp in milliseconds: 1514764800000
Human time (GMT): 2018년 January 1일 Monday AM 12:00:00
Human time (your time zone): 2018년 1월 1일 월요일 오전 9:00:00 GMT+09:00
Epoch timestamp: 1546300799
Timestamp in milliseconds: 1546300799000
Human time (GMT): 2018년 December 31일 Monday PM 11:59:59
Human time (your time zone): 2019년 1월 1일 화요일 오전 8:59:59 GMT+09:00
*/
        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); //abp

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x39ffa0c5924550db0e75030ff8513c3145d491dff2e17b8e3ea1cea7b4662ff0"); //1079274

        pchMessageStart[0] = 0xd0; //abp
        pchMessageStart[1] = 0x13; //abp
        pchMessageStart[2] = 0x09; //abp
        pchMessageStart[3] = 0x11; //abp
        nDefaultPort = 19981; //abp
        nPruneAfterHeight = 1000;

		//Epoch timestamp: 1546300800
		//Timestamp in milliseconds: 1546300800000
		//Human time (GMT): 2019년 January 1일 Tuesday AM 12:00:00
		//Human time (your time zone): 2019년 1월 1일 화요일 오전 9:00:00 GMT+09:00
        genesis = CreateGenesisBlock(1605588500, 214381, 0x1f00ffff, 1, 50 * COIN); //abp CreateGenesisBlock(1541030400, 266862, 0x1f00ffff, 1, 50 * COIN)
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00008435a6d3dd42fd2bc7407d5ff591cfd88d59d3283f50c93ccc1f381dfdfe")); //abp 0x000039c375f664b12c1bd8c7cfc8d3f943c1c5b9411e7f4be1eb4ee2be08d09c
        assert(genesis.hashMerkleRoot == uint256S("0xefd0a530a4567e676430e0a6ea6f3c7c3c2b3873769fc33ee902876634465843")); //abp 0x0ce221cf045c8ed0a9a815dc654d408635a80843f8b560ffb5517a94cdee446f

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("abpbc6.fast.kr");  //abp // Abp testnet

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,83); //abp (Y) https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,24); //abp (Y) https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "ta"; //abp

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {0, uint256S("0x00008435a6d3dd42fd2bc7407d5ff591cfd88d59d3283f50c93ccc1f381dfdfe")}, //abp 000039c375f664b12c1bd8c7cfc8d3f943c1c5b9411e7f4be1eb4ee2be08d09c"
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 493cccf2ba87ffdabd7afc0f3242c1357fdebdc0b8c7e7adc3c6dc2b1c8ca797 (height 79167)
            0, //abp
            0, //abp
            0 //abp
        };

        consensus.nLastPOWBlock = 5000;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock + 
                                    consensus.nMPoSRewardRecipients + 
                                    COINBASE_MATURITY;

        consensus.nFixUTXOCacheHFHeight=84500;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 0; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests) // activate for abp
        consensus.BIP34Hash = uint256S("0x"); //abp
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60; //abp 10 minutes //16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 1 * 10; //abp //2 * 64;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfc; //abp
        pchMessageStart[1] = 0xc1; //abp
        pchMessageStart[2] = 0xb7; //abp
        pchMessageStart[3] = 0xdc; //abp
        nDefaultPort = 29981; //abp
        nPruneAfterHeight = 1000;

		//Epoch timestamp: 1546300800
		//Timestamp in milliseconds: 1546300800000
		//Human time (GMT): 2019년 January 1일 Tuesday AM 12:00:00
		//Human time (your time zone): 2019년 1월 1일 화요일 오전 9:00:00 GMT+09:00
        genesis = CreateGenesisBlock(1541030400, 11, 0x207fffff, 1, 50 * COIN); //abp CreateGenesisBlock(1541030400, 11, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x")); //abp
        assert(genesis.hashMerkleRoot == uint256S("0x")); //abp

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("0x")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };
        consensus.nLastPOWBlock = 0x7fffffff;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = 5000;

        consensus.nFixUTXOCacheHFHeight=0;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140); //abp (y)
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,141); //abp (y)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "ycrt"; //abp
    }
};

/**
 * Regression network parameters overwrites for unit testing
 */
class CUnitTestParams : public CRegTestParams
{
public:
    CUnitTestParams()
    {
        // Activate the the BIPs for regtest as in Bitcoin
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)

        // ABP have 500 blocks of maturity, increased values for regtest in unit tests in order to correspond with it
        consensus.nSubsidyHalvingInterval = 750;
        consensus.nRuleChangeActivationThreshold = 558; // 75% for testchains
        consensus.nMinerConfirmationWindow = 744; // Faster than normal for regtest (744 instead of 2016)
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    else if (chain == CBaseChainParams::UNITTEST)
        return std::unique_ptr<CChainParams>(new CUnitTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
