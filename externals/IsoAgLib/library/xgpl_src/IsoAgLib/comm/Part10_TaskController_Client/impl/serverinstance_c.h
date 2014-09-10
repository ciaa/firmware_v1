/*
  serverinstance_c.h: Instance of a TC or DL

  (C) Copyright 2013 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/tcclientconnection_c.h>

#include <list>


namespace __IsoAgLib {

  class ServerInstance_c: public SchedulerTask_c {
    public:
      ServerInstance_c( const IsoItem_c& it, IsoAgLib::ProcData::RemoteType_t type );
      ~ServerInstance_c();

      void processStatus( uint8_t status );

      bool isAlive() const;

      const IsoItem_c& getIsoItem() const {
        return m_isoItem;
      }

      void processMsgNonGlobal( const ProcessPkg_c& pkg );

      void addConnection( TcClientConnection_c& c );
      void removeConnection( TcClientConnection_c& c );

      int32_t getLastStatusTime() const {
        return m_lastTcStateReceivedTime;
      }
      uint8_t getLastServerState() const {
        return m_lastTcState;
      }
      bool getLastActiveTaskTC() const {
        return m_lastActiveTaskTC;
      }

      IsoAgLib::ProcData::RemoteType_t getEcuType() const {
        return m_type;
      }

    private:
      void timeEvent();

    private:
      const IsoItem_c& m_isoItem;

      bool m_tcAliveCached;
      
      bool m_lastActiveTaskTC;
      uint8_t m_lastTcState;
      int32_t m_lastTcStateReceivedTime;
      IsoAgLib::ProcData::RemoteType_t m_type;

      STL_NAMESPACE::list<TcClientConnection_c*> m_connections;
    private:
      ServerInstance_c(ServerInstance_c const &);
      ServerInstance_c &operator=(ServerInstance_c const &);
  };

}
