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

#ifndef JBCOIN_NET_RPCSUB_H_INCLUDED
#define JBCOIN_NET_RPCSUB_H_INCLUDED

#include <jbcoin/core/JobQueue.h>
#include <jbcoin/net/InfoSub.h>
#include <jbcoin/core/Stoppable.h>
#include <boost/asio/io_service.hpp>

namespace jbcoin {

/** Subscription object for JSON RPC. */
class RPCSub : public InfoSub
{
public:
    virtual void setUsername (std::string const& strUsername) = 0;
    virtual void setPassword (std::string const& strPassword) = 0;

protected:
    explicit RPCSub (InfoSub::Source& source);
};

// VFALCO Why is the io_service needed?
std::shared_ptr<RPCSub> make_RPCSub (
    InfoSub::Source& source, boost::asio::io_service& io_service,
    JobQueue& jobQueue, std::string const& strUrl,
    std::string const& strUsername, std::string const& strPassword,
    Logs& logs);

} // jbcoin

#endif
