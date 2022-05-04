// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include <chainparamsbase.h>
#include <consensus/params.h>
#include <netaddress.h>
#include <primitives/block.h>
#include <protocol.h>
#include <util/hash_type.h>
#include <libethashseal/GenesisInfo.h>

#include <memory>
#include <string>
#include <vector>

typedef std::map<int, uint256> MapCheckpoints;

struct CCheckpointData {
    MapCheckpoints mapCheckpoints;

    int GetHeight() const {
        const auto& final_checkpoint = mapCheckpoints.rbegin();
        return final_checkpoint->first /* height */;
    }
};

struct AssumeutxoHash : public BaseHash<uint256> {
    explicit AssumeutxoHash(const uint256& hash) : BaseHash(hash) {}
};

/**
 * Holds configuration for use during UTXO snapshot load and validation. The contents
 * here are security critical, since they dictate which UTXO snapshots are recognized
 * as valid.
 */
struct AssumeutxoData {
    //! The expected hash of the deserialized UTXO set.
    const AssumeutxoHash hash_serialized;

    //! Used to populate the nChainTx value, which is used during BlockManager::LoadBlockIndex().
    //!
    //! We need to hardcode the value here because this is computed cumulatively using block data,
    //! which we do not necessarily have at the time of snapshot load.
    const unsigned int nChainTx;
};

using MapAssumeutxo = std::map<int, const AssumeutxoData>;

/**
 * Holds various statistics on transactions within a chain. Used to estimate
 * verification progress during chain sync.
 *
 * See also: CChainParams::TxData, GuessVerificationProgress.
 */
struct ChainTxData {
    int64_t nTime;    //!< UNIX timestamp of last known number of transactions
    int64_t nTxCount; //!< total number of transactions between genesis and that timestamp
    double dTxRate;   //!< estimated number of transactions per second after that timestamp
};

const std::string DELEGATIONS_CONTRACT_CODE="608060405234801561001057600080fd5b50600436106100415760003560e01c80633d666e8b146100465780634c0e968c14610050578063bffe348614610138575b600080fd5b61004e61023c565b005b6101366004803603606081101561006657600080fd5b81019080803573ffffffffffffffffffffffffffffffffffffffff169060200190929190803560ff169060200190929190803590602001906401000000008111156100b057600080fd5b8201836020820111156100c257600080fd5b803590602001918460018302840111640100000000831117156100e457600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600081840152601f19601f820116905080830192505050505050509192919290505050610428565b005b61017a6004803603602081101561014e57600080fd5b81019080803573ffffffffffffffffffffffffffffffffffffffff169060200190929190505050610dbb565b604051808573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020018460ff1660ff16815260200183815260200180602001828103825283818151815260200191508051906020019080838360005b838110156101fe5780820151818401526020810190506101e3565b50505050905090810190601f16801561022b5780820380516001836020036101000a031916815260200191505b509550505050505060405180910390f35b60008060003373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206001015414156102d7576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040180806020018281038252603281526020018061174a6032913960400191505060405180910390fd5b3373ffffffffffffffffffffffffffffffffffffffff166000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff167f7fe28d2d0b16cf95b5ea93f4305f89133b3892543e616381a1336fc1e7a01fa060405160405180910390a36000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020600080820160006101000a81549073ffffffffffffffffffffffffffffffffffffffff02191690556000820160146101000a81549060ff021916905560018201600090556002820160006104249190611564565b5050565b3373ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1614156104ca576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260178152602001807f43616e6e6f742064656c656761746520746f2073656c6600000000000000000081525060200191505060405180910390fd5b600073ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff16141561056d576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260168152602001807f496e76616c6964207374616b657220616464726573730000000000000000000081525060200191505060405180910390fd5b60648260ff1611156105ca576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260348152602001806116966034913960400191505060405180910390fd5b6041815114610624576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260248152602001806116ca6024913960400191505060405180910390fd5b61062f818433610eb0565b6106a1576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260158152602001807f496e76616c696420506f44207369676e6174757265000000000000000000000081525060200191505060405180910390fd5b8273ffffffffffffffffffffffffffffffffffffffff166000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1614801561079057508160ff166000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000160149054906101000a900460ff1660ff16145b80156108c55750806040516020018082805190602001908083835b602083106107ce57805182526020820191506020810190506020830392506107ab565b6001836020036101000a038019825116818451168082178552505050505050905001915050604051602081830303815290604052805190602001206000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060020160405160200180828054600181600116156101000203166002900480156108a85780601f106108865761010080835404028352918201916108a8565b820191906000526020600020905b815481529060010190602001808311610894575b505091505060405160208183030381529060405280519060200120145b15610938576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004018080602001828103825260198152602001807f44656c65676174696f6e20616c7265616479206578697374730000000000000081525060200191505060405180910390fd5b60008060003373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206001015414158015610a1757508273ffffffffffffffffffffffffffffffffffffffff166000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1614155b15610ad8573373ffffffffffffffffffffffffffffffffffffffff166000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff167f7fe28d2d0b16cf95b5ea93f4305f89133b3892543e616381a1336fc1e7a01fa060405160405180910390a35b3373ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff167fa23803f3b2b56e71f2921c22b23c32ef596a439dbe03f7250e6b58a30eb910b5844385604051808460ff1660ff16815260200183815260200180602001828103825283818151815260200191508051906020019080838360005b83811015610b7c578082015181840152602081019050610b61565b50505050905090810190601f168015610ba95780820380516001836020036101000a031916815260200191505b5094505050505060405180910390a3826000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000160006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff160217905550816000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000160146101000a81548160ff021916908360ff160217905550436000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060010181905550806000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206002019080519060200190610d2d9291906115ac565b50621e94805a1015610d8a576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040180806020018281038252605c8152602001806116ee605c913960600191505060405180910390fd5b60005a90505b600115610db5576000600181905550621e84805a820310610db057610db5565b610d90565b50505050565b60006020528060005260406000206000915090508060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff16908060000160149054906101000a900460ff1690806001015490806002018054600181600116156101000203166002900480601f016020809104026020016040519081016040528092919081815260200182805460018160011615610100020316600290048015610ea65780601f10610e7b57610100808354040283529160200191610ea6565b820191906000526020600020905b815481529060010190602001808311610e8957829003601f168201915b5050505050905084565b600060606040518060400160405280601781526020017f155174756d205369676e6564204d6573736167653a0a2800000000000000000081525090506060610ef7856111ad565b90506000610f12610f0b88600060016113db565b6000611467565b90506000610f2d610f2689600160206113db565b600061148f565b90506000610f48610f418a602160206113db565b600061148f565b9050600060028087876040516020018083805190602001908083835b60208310610f875780518252602082019150602081019050602083039250610f64565b6001836020036101000a03801982511681845116808217855250505050505090500182805190602001908083835b60208310610fd85780518252602082019150602081019050602083039250610fb5565b6001836020036101000a038019825116818451168082178552505050505050905001925050506040516020818303038152906040526040518082805190602001908083835b60208310611040578051825260208201915060208101905060208303925061101d565b6001836020036101000a038019825116818451168082178552505050505050905001915050602060405180830381855afa158015611082573d6000803e3d6000fd5b5050506040513d602081101561109757600080fd5b8101908080519060200190929190505050604051602001808281526020019150506040516020818303038152906040526040518082805190602001908083835b602083106110fa57805182526020820191506020810190506020830392506110d7565b6001836020036101000a038019825116818451168082178552505050505050905001915050602060405180830381855afa15801561113c573d6000803e3d6000fd5b5050506040513d602081101561115157600080fd5b810190808051906020019092919050505090508773ffffffffffffffffffffffffffffffffffffffff16611187828686866114b7565b73ffffffffffffffffffffffffffffffffffffffff161496505050505050509392505050565b606060008273ffffffffffffffffffffffffffffffffffffffff1660001b905060606040518060400160405280601081526020017f30313233343536373839616263646566000000000000000000000000000000008152509050606060286040519080825280601f01601f19166020018201604052801561123d5781602001600182028038833980820191505090505b50905060008090505b60148110156113655782600485600c84016020811061126157fe5b1a60f81b7effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916901c60f81c60ff168151811061129957fe5b602001015160f81c60f81b8260028302815181106112b357fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a90535082600f60f81b85600c8401602081106112f757fe5b1a60f81b1660f81c60ff168151811061130c57fe5b602001015160f81c60f81b82600283026001018151811061132957fe5b60200101907effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916908160001a9053508080600101915050611246565b50806040516020018082805190602001908083835b6020831061139d578051825260208201915060208101905060208303925061137a565b6001836020036101000a0380198251168184511680821785525050505050509050019150506040516020818303038152906040529350505050919050565b6060818301845110156113ed57600080fd5b606082156000811461140a5760405191506020820160405261145b565b6040519150601f8416801560200281840101858101878315602002848b0101015b81831015611448578051835260208301925060208101905061142b565b50868552601f19601f8301166040525050505b50809150509392505050565b6000600182018351101561147a57600080fd5b60008260018501015190508091505092915050565b600060208201835110156114a257600080fd5b60008260208501015190508091505092915050565b60006114c161162c565b8560001c816000600481106114d257fe5b6020020181815250508460ff16816001600481106114ec57fe5b6020020181815250508360001c8160026004811061150657fe5b6020020181815250508260001c8160036004811061152057fe5b60200201818152505061153161164e565b6020816080846085600019fa61154657600080fd5b8060006001811061155357fe5b602002015192505050949350505050565b50805460018160011615610100020316600290046000825580601f1061158a57506115a9565b601f0160209004906000526020600020908101906115a89190611670565b5b50565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f106115ed57805160ff191683800117855561161b565b8280016001018555821561161b579182015b8281111561161a5782518255916020019190600101906115ff565b5b5090506116289190611670565b5090565b6040518060800160405280600490602082028038833980820191505090505090565b6040518060200160405280600190602082028038833980820191505090505090565b61169291905b8082111561168e576000816000905550600101611676565b5090565b9056fe496e76616c696420666565202520286d75737420626520616e20696e7465676572206265747765656e203020616e642031303029506f4420696e76616c69642073697a652c2073686f756c642062652036352062797465734e6f7420656e6f75676820676173206c65667420746f20636f6e73756d652c20746865207265636f6d6d656e64656420676173206c696d697420746f2063616c6c20746869732066756e6374696f6e20697320322c3235302c30303044656c65676174696f6e20646f6573206e6f742065786973742c20736f2069742063616e6e6f742062652072656d6f766564a265627a7a723158204b0af0e10415b6d9014f408a210216f51cf7797f82c4fdeb31ee65c408f42f3364736f6c634300050f0032";

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system.
 */
class CChainParams
{
public:
    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,
        EXT_PUBLIC_KEY,
        EXT_SECRET_KEY,

        MAX_BASE58_TYPES
    };

    const Consensus::Params& GetConsensus() const { return consensus; }
    const CMessageHeader::MessageStartChars& MessageStart() const { return pchMessageStart; }
    uint16_t GetDefaultPort() const { return nDefaultPort; }
    uint16_t GetDefaultPort(Network net) const
    {
        return net == NET_I2P ? I2P_SAM31_PORT : GetDefaultPort();
    }
    uint16_t GetDefaultPort(const std::string& addr) const
    {
        CNetAddr a;
        return a.SetSpecial(addr) ? GetDefaultPort(a.GetNetwork()) : GetDefaultPort();
    }

    const CBlock& GenesisBlock() const { return genesis; }
    /** Default value for -checkmempool and -checkblockindex argument */
    bool DefaultConsistencyChecks() const { return fDefaultConsistencyChecks; }
    /** Policy: Filter transactions that do not match well-defined patterns */
    bool RequireStandard() const { return fRequireStandard; }
    /** If this chain is exclusively used for testing */
    bool IsTestChain() const { return m_is_test_chain; }
    /** If this chain allows time to be mocked */
    bool IsMockableChain() const { return m_is_mockable_chain; }
    uint64_t PruneAfterHeight() const { return nPruneAfterHeight; }
    /** Minimum free space (in GB) needed for data directory */
    uint64_t AssumedBlockchainSize() const { return m_assumed_blockchain_size; }
    /** Minimum free space (in GB) needed for data directory when pruned; Does not include prune target*/
    uint64_t AssumedChainStateSize() const { return m_assumed_chain_state_size; }
    /** Whether it is possible to mine blocks on demand (no retargeting) */
    bool MineBlocksOnDemand() const { return fMineBlocksOnDemand; }
    /** Return the network string */
    std::string NetworkIDString() const { return strNetworkID; }
    /** Return the list of hostnames to look up for DNS seeds */
    const std::vector<std::string>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const std::string& Bech32HRP() const { return bech32_hrp; }
    const std::vector<uint8_t>& FixedSeeds() const { return vFixedSeeds; }
    const CCheckpointData& Checkpoints() const { return checkpointData; }
    std::string EVMGenesisInfo() const;
    std::string EVMGenesisInfo(int nHeight) const;
    std::string EVMGenesisInfo(const dev::eth::EVMConsensus& evmConsensus) const;
    void UpdateOpSenderBlockHeight(int nHeight);
    void UpdateBtcEcrecoverBlockHeight(int nHeight);
    void UpdateConstantinopleBlockHeight(int nHeight);
    void UpdateDifficultyChangeBlockHeight(int nHeight);
    void UpdateOfflineStakingBlockHeight(int nHeight);
    void UpdateDelegationsAddress(const uint160& address);
    void UpdateLastMPoSBlockHeight(int nHeight);
    void UpdateReduceBlocktimeHeight(int nHeight);
    void UpdatePowAllowMinDifficultyBlocks(bool fValue);
    void UpdatePowNoRetargeting(bool fValue);
    void UpdatePoSNoRetargeting(bool fValue);
    void UpdateMuirGlacierHeight(int nHeight);
    bool HasHardwareWalletSupport() const { return fHasHardwareWalletSupport; }
    void UpdateLondonHeight(int nHeight);
    void UpdateTaprootHeight(int nHeight);
    //! Get allowed assumeutxo configuration.
    //! @see ChainstateManager
    const MapAssumeutxo& Assumeutxo() const { return m_assumeutxo_data; }

    const ChainTxData& TxData() const { return chainTxData; }
protected:
    dev::eth::Network GetEVMNetwork() const;
    CChainParams() {}

    Consensus::Params consensus;
    CMessageHeader::MessageStartChars pchMessageStart;
    uint16_t nDefaultPort;
    uint64_t nPruneAfterHeight;
    uint64_t m_assumed_blockchain_size;
    uint64_t m_assumed_chain_state_size;
    std::vector<std::string> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    std::string bech32_hrp;
    std::string strNetworkID;
    CBlock genesis;
    std::vector<uint8_t> vFixedSeeds;
    bool fDefaultConsistencyChecks;
    bool fRequireStandard;
    bool fMineBlocksOnDemand;
    bool m_is_test_chain;
    bool m_is_mockable_chain;
    CCheckpointData checkpointData;
    MapAssumeutxo m_assumeutxo_data;
    ChainTxData chainTxData;
    bool fHasHardwareWalletSupport;
};

/**
 * Creates and returns a std::unique_ptr<CChainParams> of the chosen chain.
 * @returns a CChainParams* of the chosen chain.
 * @throws a std::runtime_error if the chain is not supported.
 */
std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const std::string& chain);

/**
 * Return the currently selected parameters. This won't change after app
 * startup, except for unit tests.
 */
const CChainParams &Params();

/**
 * Sets the params returned by Params() to those for the given chain name.
 * @throws std::runtime_error when the chain is not supported.
 */
void SelectParams(const std::string& chain);

/**
 * Allows modifying the Op Sender block height regtest parameter.
 */
void UpdateOpSenderBlockHeight(int nHeight);

/**
 * Allows modifying the btc_ecrecover block height regtest parameter.
 */
void UpdateBtcEcrecoverBlockHeight(int nHeight);

/**
 * Allows modifying the constantinople block height regtest parameter.
 */
void UpdateConstantinopleBlockHeight(int nHeight);

/**
 * Allows modifying the difficulty change block height regtest parameter.
 */
void UpdateDifficultyChangeBlockHeight(int nHeight);

/**
 * Allows modifying the offline staking block height regtest parameter.
 */
void UpdateOfflineStakingBlockHeight(int nHeight);

/**
 * Allows modifying the delegations address regtest parameter.
 */
void UpdateDelegationsAddress(const uint160& address);

/**
 * @brief UpdateLastMPoSBlockHeight Last mpos block height
 * @param nHeight Block height
 */
void UpdateLastMPoSBlockHeight(int nHeight);

/**
 * Allows modifying the reduce block time height regtest parameter.
 */
void UpdateReduceBlocktimeHeight(int nHeight);

/**
 * Allows modifying the pow allow for min difficulty blocks regtest parameter.
 */
void UpdatePowAllowMinDifficultyBlocks(bool fValue);

/**
 * Allows modifying the pow no retargeting regtest parameter.
 */
void UpdatePowNoRetargeting(bool fValue);

/**
 * Allows modifying the pos no retargeting regtest parameter.
 */
void UpdatePoSNoRetargeting(bool fValue);

/**
 * Allows modifying the muir glacier block height regtest parameter.
 */
void UpdateMuirGlacierHeight(int nHeight);

/**
 * Allows modifying the london block height regtest parameter.
 */
void UpdateLondonHeight(int nHeight);

/**
 * Allows modifying the taproot block height regtest parameter.
 */
void UpdateTaprootHeight(int nHeight);

#endif // BITCOIN_CHAINPARAMS_H
