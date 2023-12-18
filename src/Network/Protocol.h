#pragma once

#include <Utils/BinaryBuffer.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost::asio::ip;

namespace Network
{
    class Protocol
    {
    private:

        using Socket    = tcp::socket;
        using Endpoint  = tcp::endpoint;
        using Address   = boost::asio::ip::address;
        using Context   = boost::asio::io_context;

        static constexpr std::string_view	SERVER_ADDRESS = "127.0.0.1";
        static constexpr Int32				SERVER_PORT = 7666;

        UPtr<Utils::BinaryBuffer> mIncomingBuffer = nullptr;
        UPtr<Utils::BinaryBuffer> mOutgoingBuffer = nullptr;

        Context mContext;
        Socket mSocket = Socket(mContext.get_executor());        
        
        std::thread mContextThread;

        Address     mAddress = Address::from_string(SERVER_ADDRESS.data());
        Endpoint    mEndpoint = Endpoint(mAddress, SERVER_PORT);

        enum class ServerPacketID
        {
            logged, // LOGGED
            RemoveDialogs, // QTDL
            RemoveCharDialog, // QDL
            NavigateToggle, // NAVEG
            Disconnect, // FINOK
            CommerceEnd, // FINCOMOK
            BankEnd, // FINBANOK
            CommerceInit, // INITCOM
            BankInit, // INITBANCO
            UserCommerceInit, // INITCOMUSU
            UserCommerceEnd, // FINCOMUSUOK
            UserOfferConfirm,
            CommerceChat,
            ShowBlacksmithForm, // SFH
            ShowCarpenterForm, // SFC
            UpdateSta, // ASS
            UpdateMana, // ASM
            UpdateHP, // ASH
            UpdateGold, // ASG
            UpdateBankGold,
            UpdateExp, // ASE
            ChangeMap, // CM
            PosUpdate, // PU
            ChatOverHead, // ||
            ConsoleMsg, // || - Beware!! its the same as above, but it was properly splitted
            GuildChat, // |+
            ShowMessageBox, // !!
            UserIndexInServer, // IU
            UserCharIndexInServer, // IP
            CharacterCreate, // CC
            CharacterRemove, // BP
            CharacterChangeNick,
            CharacterMove, // MP, +, * and _, //
            ForceCharMove,
            CharacterChange, // CP
            ObjectCreate, // HO
            ObjectDelete, // BO
            BlockPosition, // BQ
            PlayMIDI, // TM
            PlayWave, // TW
            guildList, // GL
            AreaChanged, // CA
            PauseToggle, // BKW
            RainToggle, // LLU
            CreateFX, // CFX
            UpdateUserStats, // EST
            WorkRequestTarget, // T01
            ChangeInventorySlot, // CSI
            ChangeBankSlot, // SBO
            ChangeSpellSlot, // SHS
            Atributes, // ATR
            BlacksmithWeapons, // LAH
            BlacksmithArmors, // LAR
            CarpenterObjects, // OBR
            RestOK, // DOK
            ErrorMsg, // ERR
            Blind, // CEGU
            Dumb, // DUMB
            ShowSignal, // MCAR
            ChangeNPCInventorySlot, // NPCI
            UpdateHungerAndThirst, // EHYS
            Fame, // FAMA
            MiniStats, // MEST
            LevelUp, // SUNI
            AddForumMsg, // FMSG
            ShowForumForm, // MFOR
            SetInvisible, // NOVER
            DiceRoll, // DADOS
            MeditateToggle, // MEDOK
            BlindNoMore, // NSEGUE
            DumbNoMore, // NESTUP
            SendSkills, // SKILLS
            TrainerCreatureList, // LSTCRI
            guildNews, // GUILDNE
            OfferDetails, // PEACEDE & ALLIEDE
            AlianceProposalsList, // ALLIEPR
            PeaceProposalsList, // PEACEPR
            CharacterInfo, // CHRINFO
            GuildLeaderInfo, // LEADERI
            GuildMemberInfo,
            GuildDetails, // CLANDET
            ShowGuildFundationForm, // SHOWFUN
            ParalizeOK, // PARADOK
            ShowUserRequest, // PETICIO
            TradeOK, // TRANSOK
            BankOK, // BANCOOK
            ChangeUserTradeSlot, // COMUSUINV
            SendNight, // NOC
            Pong,
            UpdateTagAndStatus,

            //GM messages
            SpawnList, // SPL
            ShowSOSForm, // MSOS
            ShowMOTDEditionForm, // ZMOTD
            ShowGMPanelForm, // ABPANEL
            UserNameList, // LISTUSU
            ShowDenounces,
            RecordList,
            RecordDetails,

            ShowGuildAlign,
            ShowPartyForm,
            UpdateStrenghtAndDexterity,
            UpdateStrenght,
            UpdateDexterity,
            AddSlots,
            MultiMessage,
            StopWorking,
            CancelOfferItem
        };

        enum class ClientPacketID
        {
            LoginExistingChar, //OLOGIN
            ThrowDices, //TIRDAD
            LoginNewChar, //NLOGIN
            Talk, //;
            Yell, //-
            Whisper, //"\"
            Walk, //M
            RequestPositionUpdate, //RPU
            Attack, //AT
            PickUp, //AG
            SafeToggle, //SEG & SEG  (SEG's behaviour has to be coded in the client)
            ResuscitationSafeToggle,
            RequestGuildLeaderInfo, //GLINFO
            RequestAtributes, //ATR
            RequestFame, //FAMA
            RequestSkills, //ESKI
            RequestMiniStats, //FEST
            CommerceEnd, //FINCOM
            UserCommerceEnd, //FINCOMUSU
            UserCommerceConfirm,
            CommerceChat,
            BankEnd, //FINBAN
            UserCommerceOk, //COMUSUOK
            UserCommerceReject, //COMUSUNO
            Drop, //TI
            CastSpell, //LH
            LeftClick, //LC
            DoubleClick, //RC
            Work, //UK
            UseSpellMacro, //UMH
            UseItem, //USA
            CraftBlacksmith, //CNS
            CraftCarpenter, //CNC
            WorkLeftClick, //WLC
            CreateNewGuild, //CIG
            SpellInfo, //INFS
            EquipItem, //EQUI
            ChangeHeading, //CHEA
            ModifySkills, //SKSE
            Train, //ENTR
            CommerceBuy, //COMP
            BankExtractItem, //RETI
            CommerceSell, //VEND
            BankDeposit, //DEPO
            ForumPost, //DEMSG
            MoveSpell, //DESPHE
            MoveBank,
            ClanCodexUpdate, //DESCOD
            UserCommerceOffer, //OFRECER
            GuildAcceptPeace, //ACEPPEAT
            GuildRejectAlliance, //RECPALIA
            GuildRejectPeace, //RECPPEAT
            GuildAcceptAlliance, //ACEPALIA
            GuildOfferPeace, //PEACEOFF
            GuildOfferAlliance, //ALLIEOFF
            GuildAllianceDetails, //ALLIEDET
            GuildPeaceDetails, //PEACEDET
            GuildRequestJoinerInfo, //ENVCOMEN
            GuildAlliancePropList, //ENVALPRO
            GuildPeacePropList, //ENVPROPP
            GuildDeclareWar, //DECGUERR
            GuildNewWebsite, //NEWWEBSI
            GuildAcceptNewMember, //ACEPTARI
            GuildRejectNewMember, //RECHAZAR
            GuildKickMember, //ECHARCLA
            GuildUpdateNews, //ACTGNEWS
            GuildMemberInfo, //1HRINFO<
            GuildOpenElections, //ABREELEC
            GuildRequestMembership, //SOLICITUD
            GuildRequestDetails, //CLANDETAILS
            Online, //ONLINE
            Quit, //SALIR
            GuildLeave, //SALIRCLAN
            RequestAccountState, //BALANCE
            PetStand, //QUIETO
            PetFollow, //ACOMPAÑAR
            ReleasePet, //LIBERAR
            TrainList, //ENTRENAR
            Rest, //DESCANSAR
            Meditate, //MEDITAR
            Resucitate, //RESUCITAR
            Heal, //CURAR
            Help, //AYUDA
            RequestStats, //EST
            CommerceStart, //COMERCIAR
            BankStart, //BOVEDA
            Enlist, //ENLISTAR
            Information, //INFORMACION
            Reward, //RECOMPENSA
            RequestMOTD, //MOTD
            Uptime, //UPTIME
            PartyLeave, //SALIRPARTY
            PartyCreate, //CREARPARTY
            PartyJoin, //PARTY
            Inquiry, //ENCUESTA ( with no params )
            GuildMessage, //CMSG
            PartyMessage, //PMSG
            CentinelReport, //CENTINELA
            GuildOnline, //ONLINECLAN
            PartyOnline, //ONLINEPARTY
            CouncilMessage, //BMSG
            RoleMasterRequest, //ROL
            GMRequest, //GM
            bugReport, //_BUG
            ChangeDescription, //DESC
            GuildVote, //VOTO
            Punishments, //PENAS
            ChangePassword, //CONTRASEÑA
            Gamble, //APOSTAR
            InquiryVote, //ENCUESTA ( with parameters )
            LeaveFaction, //RETIRAR ( with no arguments )
            BankExtractGold, //RETIRAR ( with arguments )
            BankDepositGold, //DEPOSITAR
            Denounce, //DENUNCIAR
            GuildFundate, //FUNDARCLAN
            GuildFundation,
            PartyKick, //ECHARPARTY
            PartySetLeader, //PARTYLIDER
            PartyAcceptMember, //ACCEPTPARTY
            Ping, //PING

            RequestPartyForm,
            ItemUpgrade,
            GMCommands,
            InitCrafting,
            Home,
            ShowGuildNews,
            ShareNpc, //COMPARTIRNPC
            StopSharingNpc, //NOCOMPARTIRNPC
            Consultation,
            MoveItem, //Drag and drop
        };
        
        void HandleMultiMessage();
        void HandleLogged();
        void HandleRemoveDialogs();
        void HandleRemoveCharDialog();
        void HandleNavigateToggle();
        void HandleDisconnect();
        void HandleCommerceEnd();
        void HandleBankEnd();
        void HandleCommerceInit();
        void HandleBankInit();
        void HandleUserCommerceInit();
        void HandleUserCommerceEnd();
        void HandleUserOfferConfirm();
        void HandleShowBlacksmithForm();
        void HandleShowCarpenterForm();
        void HandleNPCSwing();
        void HandleNPCKillUser();
        void HandleBlockedWithShieldUser();
        void HandleBlockedWithShieldOther();
        void HandleUserSwing();
        void HandleSafeModeOn();
        void HandleSafeModeOff();
        void HandleResuscitationSafeOff();
        void HandleResuscitationSafeOn();
        void HandleNobilityLost();
        void HandleCantUseWhileMeditating();
        void HandleUpdateSta();
        void HandleUpdateMana();
        void HandleUpdateHP();
        void HandleUpdateGold();
        void HandleUpdateBankGold();
        void HandleUpdateExp();
        void HandleUpdateStrenghtAndDexterity();
        void HandleUpdateStrenght();
        void HandleUpdateDexterity();
        void HandleChangeMap();
        void HandlePosUpdate();
        void HandleNPCHitUser();
        void HandleUserHitNPC();
        void HandleUserAttackedSwing();
        void HandleUserHittedByUser();
        void HandleUserHittedUser();
        void HandleChatOverHead();
        void HandleConsoleMessage();
        void HandleGuildChat();
        void HandleCommerceChat();
        void HandleShowMessageBox();
        void HandleUserIndexInServer();
        void HandleUserCharIndexInServer();
        void HandleCharacterCreate();
        void HandleCharacterChangeNick();
        void HandleCharacterRemove();
        void HandleCharacterMove();
        void HandleForceCharMove();
        void HandleCharacterChange();
        void HandleObjectCreate();
        void HandleObjectDelete();
        void HandleBlockPosition();
        void HandlePlayMIDI();
        void HandlePlayWave();
        void HandleGuildList();
        void HandleAreaChanged();
        void HandlePauseToggle();
        void HandleRainToggle();
        void HandleCreateFX();
        void HandleUpdateUserStats();
        void HandleWorkRequestTarget();
        void HandleChangeInventorySlot();
        void HandleAddSlots();
        void HandleStopWorking();
        void HandleCancelOfferItem();
        void HandleChangeBankSlot();
        void HandleChangeSpellSlot();
        void HandleAtributes();
        void HandleBlacksmithWeapons();
        void HandleBlacksmithArmors();
        void HandleCarpenterObjects();
        void HandleRestOK();
        void HandleErrorMessage();
        void HandleBlind();
        void HandleDumb();
        void HandleShowSignal();
        void HandleChangeNPCInventorySlot();
        void HandleUpdateHungerAndThirst();
        void HandleFame();
        void HandleMiniStats();
        void HandleLevelUp();
        void HandleAddForumMessage();
        void HandleShowForumForm();
        void HandleSetInvisible();
        void HandleDiceRoll();
        void HandleMeditateToggle();
        void HandleBlindNoMore();
        void HandleDumbNoMore();
        void HandleSendSkills();
        void HandleTrainerCreatureList();
        void HandleGuildNews();
        void HandleOfferDetails();
        void HandleAlianceProposalsList();
        void HandlePeaceProposalsList();
        void HandleCharacterInfo();
        void HandleGuildLeaderInfo();
        void HandleGuildDetails();
        void HandleShowGuildAlign();
        void HandleShowGuildFundationForm();
        void HandleParalizeOK();
        void HandleShowUserRequest();
        void HandleTradeOK();
        void HandleBankOK();
        void HandleChangeUserTradeSlot();
        void HandleSendNight();
        void HandleSpawnList();
        void HandleShowSOSForm();
        void HandleShowDenounces();
        void HandleShowPartyForm();
        void HandleShowMOTDEditionForm();
        void HandleShowGMPanelForm();
        void HandleUserNameList();
        void HandlePong();
        void HandleGuildMemberInfo();
        void HandleUpdateTagAndStatus();
        
    public:
        Protocol();
        ~Protocol();

        Bool Connect();
        Bool Disconnect();
        Bool IsConnected();

        void HandleIncomingData(const boost::system::error_code& error, std::size_t bytes_transferred);
        void SendData();

        void WriteLoginExistingChar(const char* userName, const char* userPass);
        void WriteThrowDices();
        void WriteLoginNewChar(const char* userName, const char* userPass);
        void WriteTalk(String chat);
        void WriteYell(String chat);
        void WriteWhisper(String CharName, String chat);
        void WriteWalk(UInt8 Heading);
        void WriteRequestPositionUpdate();
        void WriteAttack();
        void WritePickUp();
        void WriteSafeToggle();
        void WriteResuscitationToggle();
        void WriteRequestGuildLeaderInfo();
        void WriteRequestPartyForm();
        void WriteItemUpgrade(Int32 ItemIndex);
        void WriteRequestAtributes();
        void WriteRequestFame();
        void WriteRequestSkills();
        void WriteRequestMiniStats();
        void WriteCommerceEnd();
        void WriteUserCommerceEnd();
        void WriteUserCommerceConfirm();
        void WriteBankEnd();
        void WriteUserCommerceOk();
        void WriteUserCommerceReject();
        void WriteDrop(UInt8 slot, Int32 Amount);
        void WriteCastSpell(UInt8 slot);
        void WriteLeftClick(UInt8 X, UInt8 Y);
        void WriteDoubleClick(UInt8 X, UInt8 Y);
        void WriteWork(UInt8 Skill);
        void WriteUseSpellMacro();
        void WriteUseItem(UInt8 slot);
        void WriteCraftBlacksmith(Int32 Item);
        void WriteCraftCarpenter(Int32 Item);
        void WriteShowGuildNews();
        void WriteWorkLeftClick(UInt8 X, UInt8 Y, UInt8 Skill);
        void WriteCreateNewGuild(String Desc, String Name, String Site); //TODO:, ByRef Codex() As String
        void WriteSpellInfo(UInt8 slot);
        void WriteEquipItem(UInt8 slot);
        void WriteChangeHeading(UInt8 Heading);
        void WriteModifySkills(); //TODO: ByRef skillEdt() As Byte
        void WriteTrain(UInt8 creature);
        void WriteCommerceBuy(UInt8 slot, Int32 Amount);
        void WriteBankExtractItem(UInt8 slot, Int32 Amount);
        void WriteCommerceSell(UInt8 slot, Int32 Amount);
        void WriteBankDeposit(UInt8 slot, Int32 Amount);
        void WriteForumPost(String Title, String Message, UInt8 ForumMsgType);
        void WriteMoveSpell(Bool upwards, UInt8 slot);
        void WriteMoveBank(Bool upwards, UInt8 slot);
        void WriteClanCodexUpdate(String Desc); //, ByRef Codex() As String
        void WriteUserCommerceOffer(UInt8 slot, Int32 Amount, UInt8 OfferSlot);
        void WriteCommerceChat(String chat);
        void WriteGuildAcceptPeace(String guild);
        void WriteGuildRejectAlliance(String guild);
        void WriteGuildRejectPeace(String guild);
        void WriteGuildAcceptAlliance(String guild);
        void WriteGuildOfferPeace(String guild, String proposal);
        void WriteGuildOfferAlliance(String guild, String proposal);
        void WriteGuildAllianceDetails(String guild);
        void WriteGuildPeaceDetails(String guild);
        void WriteGuildRequestJoinerInfo(String UserName);
        void WriteGuildAlliancePropList();
        void WriteGuildPeacePropList();
        void WriteGuildDeclareWar(String guild);
        void WriteGuildNewWebsite(String URL);
        void WriteGuildAcceptNewMember(String UserName);
        void WriteGuildRejectNewMember(String UserName, String Reason);
        void WriteGuildKickMember(String UserName);
        void WriteGuildUpdateNews(String news);
        void WriteGuildMemberInfo(String UserName);
        void WriteGuildOpenElections();
        void WriteGuildRequestMembership(String guild, String Application);
        void WriteGuildRequestDetails(String guild);
        void WriteOnline();
        void WriteQuit();
        void WriteGuildLeave();
        void WriteRequestAccountState();
        void WritePetStand();
        void WritePetFollow();
        void WriteReleasePet();
        void WriteTrainList();
        void WriteRest();
        void WriteMeditate();
        void WriteResucitate();
        void WriteConsultation();
        void WriteHeal();
        void WriteHelp();
        void WriteRequestStats();
        void WriteCommerceStart();
        void WriteBankStart();
        void WriteEnlist();
        void WriteInformation();
        void WriteReward();
        void WriteRequestMOTD();
        void WriteUpTime();
        void WritePartyLeave();
        void WritePartyCreate();
        void WritePartyJoin();
        void WriteInquiry();
        void WriteGuildMessage(String Message);
        void WritePartyMessage(String Message);
        void WriteCentinelReport(Int32 number);
        void WriteGuildOnline();
        void WritePartyOnline();
        void WriteCouncilMessage(String Message);
        void WriteRoleMasterRequest(String Message);
        void WriteGMRequest();
        void WriteBugReport(String Message);
        void WriteChangeDescription(String Desc);
        void WriteGuildVote(String UserName);
        void WritePunishments(String UserName);
        void WriteChangePassword(String oldPass, String newPass);
        void WriteGamble(Int32 Amount);
        void WriteInquiryVote(UInt8 opt);
        void WriteLeaveFaction();
        void WriteBankExtractGold(Int32 Amount);
        void WriteBankDepositGold(Int32 Amount);
        void WriteDenounce(String Message);
        void WriteGuildFundate();
        void WriteGuildFundation(UInt8 clanType);
        void WritePartyKick(String UserName);
        void WritePartySetLeader(String UserName);
        void WritePartyAcceptMember(String UserName);
        void WriteGuildMemberList(String guild);
        void WriteInitCrafting(Int32 cantidad, Int32 NroPorCiclo);
        void WriteHome();
        void WriteGMMessage(String Message);
        void WriteShowName();
        void WriteOnlineRoyalArmy();
        void WriteOnlineChaosLegion();
        void WriteGoNearby(String UserName);
        void WriteComment(String Message);
        void WriteServerTime();
        void WriteWhere(String UserName);
        void WriteCreaturesInMap(Int32 Map);
        void WriteWarpMeToTarget();
        void WriteWarpChar(String UserName, Int32 Map, UInt8 X, UInt8 Y);
        void WriteSilence(String UserName);
        void WriteSOSShowList();
        void WriteSOSRemove(String UserName);
        void WriteGoToChar(String UserName);
        void WriteInvisible();
        void WriteGMPanel();
        void WriteRequestUserList();
        void WriteWorking();
        void WriteHiding();
        void WriteJail(String UserName, String Reason, UInt8 time);
        void WriteKillNPC();
        void WriteWarnUser(String UserName, String Reason);
        void WriteEditChar(String UserName, UInt8 EditOption, String arg1, String arg2);
        void WriteRequestCharInfo(String UserName);
        void WriteRequestCharStats(String UserName);
        void WriteRequestCharGold(String UserName);
        void WriteRequestCharInventory(String UserName);
        void WriteRequestCharBank(String UserName);
        void WriteRequestCharSkills(String UserName);
        void WriteReviveChar(String UserName);
        void WriteOnlineGM();
        void WriteOnlineMap(Int32 Map);
        void WriteForgive(String UserName);
        void WriteKick(String UserName);
        void WriteExecute(String UserName);
        void WriteBanChar(String UserName, String Reason);
        void WriteUnbanChar(String UserName);
        void WriteNPCFollow();
        void WriteSummonChar(String UserName);
        void WriteSpawnListRequest();
        void WriteSpawnCreature(Int32 creatureIndex);
        void WriteResetNPCInventory();
        void WriteCleanWorld();
        void WriteServerMessage(String Message);
        void WriteMapMessage(String Message);
        void WriteNickToIP(String UserName);
        void WriteIPToNick(); //TODO: ByRef Ip() As Byte
        void WriteGuildOnlineMembers(String guild);
        void WriteTeleportCreate(Int32 Map, UInt8 X, UInt8 Y, UInt8 Radio = 0);
        void WriteTeleportDestroy();
        void WriteRainToggle();
        void WriteSetCharDescription(String Desc);
        void WriteForceMIDIToMap(UInt8 midiID, Int32 Map);
        void WriteForceWAVEToMap(UInt8 waveID, Int32 Map, UInt8 X, UInt8 Y);
        void WriteRoyalArmyMessage(String Message);
        void WriteChaosLegionMessage(String Message);
        void WriteCitizenMessage(String Message);
        void WriteCriminalMessage(String Message);
        void WriteTalkAsNPC(String Message);
        void WriteDestroyAllItemsInArea();
        void WriteAcceptRoyalCouncilMember(String UserName);
        void WriteAcceptChaosCouncilMember(String UserName);
        void WriteItemsInTheFloor();
        void WriteMakeDumb(String UserName);
        void WriteMakeDumbNoMore(String UserName);
        void WriteDumpIPTables();
        void WriteCouncilKick(String UserName);
        void WriteSetTrigger(UInt8 Trigger);
        void WriteAskTrigger();
        void WriteBannedIPList();
        void WriteBannedIPReload();
        void WriteGuildBan(String guild);
        void WriteBanIP(Bool byIp, String Nick, String Reason); //TODO: ByRef Ip() As Byte
        void WriteUnbanIP(); //TODO: ByRef Ip() As Byte
        void WriteCreateItem(Int32 ItemIndex);
        void WriteDestroyItems();
        void WriteChaosLegionKick(String UserName);
        void WriteRoyalArmyKick(String UserName);
        void WriteForceMIDIAll(UInt8 midiID);
        void WriteForceWAVEAll(UInt8 waveID);
        void WriteRemovePunishment(String UserName, UInt8 punishment, String NewText);
        void WriteTileBlockedToggle();
        void WriteKillNPCNoRespawn();
        void WriteKillAllNearbyNPCs();
        void WriteLastIP(String UserName);
        void WriteChangeMOTD();
        void WriteSetMOTD(String Message);
        void WriteSystemMessage(String Message);
        void WriteCreateNPC(Int32 NPCIndex);
        void WriteCreateNPCWithRespawn(Int32 NPCIndex);
        void WriteImperialArmour(UInt8 armourIndex, Int32 objectIndex);
        void WriteChaosArmour(UInt8 armourIndex, Int32 objectIndex);
        void WriteNavigateToggle();
        void WriteServerOpenToUsersToggle();
        void WriteTurnOffServer();
        void WriteTurnCriminal(String UserName);
        void WriteResetFactions(String UserName);
        void WriteRemoveCharFromGuild(String UserName);
        void WriteRequestCharMail(String UserName);
        void WriteAlterPassword(String UserName, String CopyFrom);
        void WriteAlterMail(String UserName, String newMail);
        void WriteAlterName(String UserName, String newName);
        void WriteToggleCentinelActivated();
        void WriteDoBackup();
        void WriteShowGuildMessages(String guild);
        void WriteSaveMap();
        void WriteChangeMapInfoPK(Bool isPK);
        void WriteChangeMapInfoNoOcultar(Bool PermitirOcultar);
        void WriteChangeMapInfoNoInvocar(Bool PermitirInvocar);
        void WriteChangeMapInfoBackup(Bool backup);
        void WriteChangeMapInfoRestricted(String res);
        void WriteChangeMapInfoNoMagic(Bool nomagic);
        void WriteChangeMapInfoNoInvi(Bool noinvi);
        void WriteChangeMapInfoNoResu(Bool noresu);
        void WriteChangeMapInfoLand(String land);
        void WriteChangeMapInfoZone(String zone);
        void WriteChangeMapInfoStealNpc(Bool forbid);
        void WriteSaveChars();
        void WriteCleanSOS();
        void WriteShowServerForm();
        void WriteShowDenouncesList();
        void WriteEnableDenounces();
        void WriteNight();
        void WriteKickAllChars();
        void WriteReloadNPCs();
        void WriteReloadServerIni();
        void WriteReloadSpells();
        void WriteReloadObjects();
        void WriteRestart();
        void WriteResetAutoUpdate();
        void WriteChatColor(UInt8 r, UInt8 g, UInt8 b);
        void WriteIgnored();
        void WriteCheckSlot(String UserName, UInt8 slot);
        void WritePing();
        void WriteShareNpc();
        void WriteStopSharingNpc();
        void WriteSetIniVar(String sLlave, String sClave, String sValor);
        void WriteCreatePretorianClan(Int32 Map, UInt8 X, UInt8 Y);
        void WriteDeletePretorianClan(Int32 Map);
        void WriteSetDialog(String dialog);
        void WriteImpersonate();
        void WriteImitate();
        void WriteRecordAddObs(UInt8 RecordIndex, String Observation);
        void WriteRecordAdd(String Nickname, String Reason);
        void WriteRecordRemove(UInt8 RecordIndex);
        void WriteRecordListRequest();
        void WriteRecordDetailsRequest(UInt8 RecordIndex);
        void HandleRecordList();
        void HandleRecordDetails();
        void WriteMoveItem(Int32 originalSlot, Int32 newSlot, UInt8 moveType);
    };
}