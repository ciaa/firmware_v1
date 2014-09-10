/*
  isoitem_c.h: object which represents an item in an iso monitor list

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISO_ITEM_H
#define ISO_ITEM_H

#include "isoname_c.h"
#include "baseitem_c.h"


namespace IsoAgLib {
  class iIsoItem_c;
}


namespace __IsoAgLib {

class IdentItem_c;

/** item class for ISO 11783 members monitor list to manage
  local (with address claim) and remote (statistic information)
  systems; utilizes BaseItem_c for basic MonitorList
  management and IsoName_c for management of the 64bit NAME field
  @short Item with services needed for ISO11783 monitor lists.
  @author Martin Wodok
  @author Dipl.-Inform. Achim Spangler (original author)
*/
class IsoItem_c : public BaseItem_c
{
private:
  IsoItem_c& operator=( const IsoItem_c& ); // not assignable!

public:
  /** copy c'tor. only used at construction for list-insertion! */
  IsoItem_c(const IsoItem_c& acrc_src);
  IsoItem_c();
  virtual ~IsoItem_c();

  const IsoName_c& isoName() const { return mc_isoName; }

  /** set all element data with one call
    @param ai32_time creation time of this item instance
    @param acrc_isoName ISOName code of this item ((deviceClass << 3) | devClInst )
    @param aui8_nr number of this item
    @param aren_status state of this ident (off, claimed address, ...)
    @param ai_multitonInst optional key for selection of IsoAgLib instance (default 0)
  */
  void set(int32_t ai32_time, const IsoName_c& acrc_isoName, uint8_t aui8_nr,
           itemState_t aren_status = IState_c::Active, int ai_multitonInst = 0 );

  /** this sets the back-reference to the associated IdentItem if this is a LOCAL Item!
      there's no need to unset the back-reference, because the local/remote state
      should be set at creation of the IsoItem. */
  void setIdentItem (IdentItem_c& acrc_identItem) { mpc_identItem = &acrc_identItem; }

  /** NOTE: Not a SchedulerTask's timeEvent!
      * find free SA or check if last SA is available
      * send adress claim
  */
  void timeEvent();

  /** process received ADDRESS_CLAIMED message to update data and react
      if needed and (e.g. change of SA, reclaim current address)
      Note: The parameters are needed here because the actual CAN package
            may already be rewritten to (due to e.g. sending an AC, etc.)
    @param ai32_pkgTime Original message timestamp.
    @param aui8_pkgSa Original message source address.
  */
  void processAddressClaimed (int32_t ai32_pkgTime, uint8_t aui8_pkgSa);

  /** send a SA claim message
   * - needed to respond on request for claimed SA fomr other nodes
   * - also needed when a local ident triggers a periodic request for SA
   * @return true -> this item has already a claimed SA -> it sent its SA; false -> didn't send SA, as not yet claimed or not local
   */
  bool sendSaClaim();

  /// @param ab_fromConflict false => Initial Address-Claim, so we need to go to "AddressClaim"-phase!
  ///                        true => go to "ClaimedAddress" state, no 250ms wait (for now) as we changed SA!
  /// @todo SOON-240 Do we really need this parameter? Should we wait 250ms on change of SA also???
  /// @todo SOON-240 Merge with sendSaClaim - create an enum for the three cases!
  void sendAddressClaim (bool ab_fromConflict);

  void setNr( uint8_t aui8_nr ) { mui8_nr = aui8_nr; }
  uint8_t nr() const { return mui8_nr; }

  /**
   * General Chandge Address function. Will set the Address and the call the necessary broadcast.
   */
  void changeAddressAndBroadcast (uint8_t aui8_newAddress);

  /**
   * some other node has stolen the Address, so we need to give up ours first.
   */
  void giveUpAddressAndBroadcast() { changeAddressAndBroadcast (0xFE); }

  IdentItem_c* getIdentItem() const { return mpc_identItem; }

  IsoAgLib::iIsoItem_c& toIisoItem_c();
  const IsoAgLib::iIsoItem_c& toConstIisoItem_c() const;

#ifdef USE_WORKING_SET
  // @todo move into own subclass????
public:
  int32_t announceTimeStamp() const { return m_wsRemoteAnnounceTime; }

  /** (Re-)Start sending the Working-Set Announce sequence
   * @return time-announce-started (=announce_key). You need this key to check for "isAnnounced(announce_key)".
   */
  int32_t startWsAnnounce();

  // check if this item is a master (i.e. the master pointer points to itself)
  // NOTE: this function need NOT to be used in combination with function getMaster()
  //       as it already checks isMaster()
  bool isMaster () const { return (m_wsSlavesAnnounced != NULL); }

  void setLocalMasterSlaves (STL_NAMESPACE::vector<IsoName_c>* apvec_slaveIsoNames);

  /// For checking if the WS-Announce is completed use the "announce key" returned from "startWsAnnounce()".
  bool isWsAnnounced (int32_t ai32_timeAnnounceStarted) const;

  bool isWsAnnouncing() const { return (m_wsLocalSlavesToClaimAddress != 0); }

  // Remote WS-handling
public:
  void processMsgWsMaster (uint8_t slaveCount, int32_t time );
  void processMsgWsMember (IsoName_c const& slaveName, int32_t time);
private:
  void checkWsRemoteAnnouncingFinished( int32_t time );
#endif

private: // methods
  /** calculate random wait time between 0 and 153msec. from NAME and time
    @return wait offset in msec. [0..153]
  */
  uint8_t calc_randomWait();

#ifdef USE_WORKING_SET
public:
  // return pointer to vector of clients
  STL_NAMESPACE::vector<IsoName_c>* getVectorOfClients() const { return m_wsSlavesAnnounced;}

private:
  /** Used for both Local and Remote items!
   * if this pointer is != NULL, this item IS a master and the list's size is the number of associated slaves.
   * if this pointer is == NULL, this item is NOT a master.
   * It may be a slave - this can be determined by searching
   * through all Items' list for this Item's IsoName!
   */
  STL_NAMESPACE::vector<IsoName_c>* m_wsSlavesAnnounced;
  STL_NAMESPACE::vector<IsoName_c>* m_wsSlavesAnnouncing;

  /**
    * == -1  waiting to announce WS-master message
    *  >  0  still so many slaves to announce..
    * ==  0  idle / announcing complete
    */
  int8_t m_wsLocalSlavesToClaimAddress;

  int32_t m_wsLocalLastCompletedAnnounceStartedTime;
  int32_t m_wsLocalCurrentAnnounceStartedTime;

  bool m_wsLocalRepeatAnnounce;

  int32_t m_wsRemoteAnnounceTime;
#endif

private:
  uint8_t mui8_nr; // SA
  IdentItem_c* mpc_identItem; // set for local items
  IsoName_c mc_isoName;
};

}

#endif
