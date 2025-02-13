//------------------------------------------------------------------------------
/*
    This file is part of jbcoind: https://github.com/jbcoin/jbcoind
    Copyright (c) 2012, 2013 JBCoin Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef JBCOIN_CORE_CONFIG_H_INCLUDED
#define JBCOIN_CORE_CONFIG_H_INCLUDED

#include <jbcoin/basics/BasicConfig.h>
#include <jbcoin/basics/base_uint.h>
#include <jbcoin/protocol/SystemParameters.h> // VFALCO Breaks levelization
#include <jbcoin/beast/net/IPEndpoint.h>
#include <boost/beast/core/string.hpp>
#include <jbcoin/beast/utility/Journal.h>
#include <boost/filesystem.hpp> // VFALCO FIX: This include should not be here
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <cstdint>
#include <map>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace jbcoin {

class Rules;

//------------------------------------------------------------------------------

enum SizedItemName
{
    siSweepInterval,
    siNodeCacheSize,
    siNodeCacheAge,
    siTreeCacheSize,
    siTreeCacheAge,
    siSLECacheSize,
    siSLECacheAge,
    siLedgerSize,
    siLedgerAge,
    siLedgerFetch,
    siHashNodeDBCache,
    siTxnDBCache,
    siLgrDBCache,
};

struct SizedItem
{
    SizedItemName   item;
    int             sizes[5];
};

//  This entire derived class is deprecated.
//  For new config information use the style implied
//  in the base class. For existing config information
//  try to refactor code to use the new style.
//
class Config : public BasicConfig
{
public:
    // Settings related to the configuration file location and directories
    static char const* const configFileName;
    static char const* const databaseDirName;
    static char const* const validatorsFileName;

    /** Returns the full path and filename of the debug log file. */
    boost::filesystem::path getDebugLogFile () const;

    /** Returns the full path and filename of the entropy seed file. */
    boost::filesystem::path getEntropyFile () const;

private:
    boost::filesystem::path CONFIG_FILE;
public:
    boost::filesystem::path CONFIG_DIR;
private:
    boost::filesystem::path DEBUG_LOGFILE;

    void load ();
    beast::Journal j_;

    bool QUIET = false;          // Minimize logging verbosity.
    bool SILENT = false;         // No output to console after startup.
    /** Operate in stand-alone mode.

        In stand alone mode:

        - Peer connections are not attempted or accepted
        - The ledger is not advanced automatically.
        - If no ledger is loaded, the default ledger with the root
          account is created.
    */
    bool                        RUN_STANDALONE = false;

    /** Determines if the server will sign a tx, given an account's secret seed.

        In the past, this was allowed, but this functionality can have security
        implications. The new default is to not allow this functionality, but
        a config option is included to enable this.
    */
    bool signingEnabled_ = false;

public:
    bool doImport = false;
    bool nodeToShard = false;
    bool validateShards = false;
    bool ELB_SUPPORT = false;

    std::vector<std::string>    IPS;                    // Peer IPs from jbcoind.cfg.
    std::vector<std::string>    IPS_FIXED;              // Fixed Peer IPs from jbcoind.cfg.
    std::vector<std::string>    SNTP_SERVERS;           // SNTP servers from jbcoind.cfg.

    enum StartUpType
    {
        FRESH,
        NORMAL,
        LOAD,
        LOAD_FILE,
        REPLAY,
        NETWORK
    };
    StartUpType                 START_UP = NORMAL;

    bool                        START_VALID = false;

    std::string                 START_LEDGER;

    // Network parameters
    int const                   TRANSACTION_FEE_BASE = 0;   // The number of fee units a reference transaction costs

    // Note: The following parameters do not relate to the UNL or trust at all
    std::size_t                 NETWORK_QUORUM = 0;         // Minimum number of nodes to consider the network present

    // Peer networking parameters
    bool                        PEER_PRIVATE = false;           // True to ask peers not to relay current IP.
    int                         PEERS_MAX = 0;

    std::chrono::seconds        WEBSOCKET_PING_FREQ = std::chrono::minutes {5};

    // Path searching
    int                         PATH_SEARCH_OLD = 7;
    int                         PATH_SEARCH = 7;
    int                         PATH_SEARCH_FAST = 2;
    int                         PATH_SEARCH_MAX = 10;

    // Validation
    boost::optional<std::size_t> VALIDATION_QUORUM;     // validations to consider ledger authoritative

    std::uint64_t                      FEE_DEFAULT = 0;
    std::uint64_t                      FEE_ACCOUNT_RESERVE = 1 * SYSTEM_CURRENCY_PARTS;
    std::uint64_t                      FEE_OWNER_RESERVE = 1 * SYSTEM_CURRENCY_PARTS;
    std::uint64_t                      FEE_OFFER = 0;

    // Node storage configuration
    std::uint32_t                      LEDGER_HISTORY = 256;
    std::uint32_t                      FETCH_DEPTH = 1000000000;
    int                         NODE_SIZE = 0;

    bool                        SSL_VERIFY = true;
    std::string                 SSL_VERIFY_FILE;
    std::string                 SSL_VERIFY_DIR;

    // Thread pool configuration
    std::size_t                 WORKERS = 0;

    // Genesis account configuration
    std::string                 GENESIS_PASSPHRASE;

    // These override the command line client settings
    boost::optional<beast::IP::Endpoint> rpc_ip;

    std::unordered_set<uint256, beast::uhash<>> features;

public:
    Config() = default;

    int getSize (SizedItemName) const;
    /* Be very careful to make sure these bool params
        are in the right order. */
    void setup (std::string const& strConf, bool bQuiet,
        bool bSilent, bool bStandalone);
    void setupControl (bool bQuiet,
        bool bSilent, bool bStandalone);

    /**
     *  Load the config from the contents of the string.
     *
     *  @param fileContents String representing the config contents.
     */
    void loadFromString (std::string const& fileContents);

    bool quiet() const { return QUIET; }
    bool silent() const { return SILENT; }
    bool standalone() const { return RUN_STANDALONE; }

    bool canSign() const { return signingEnabled_; }
};

} // jbcoin

#endif
