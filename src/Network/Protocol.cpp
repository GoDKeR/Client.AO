#include "Protocol.h"

namespace Network
{
	Protocol::Protocol()
	{
		mIncomingBuffer = std::make_unique<Utils::BinaryBuffer>();
		mOutgoingBuffer = std::make_unique<Utils::BinaryBuffer>();

		//boost::system::error_code error_code;

		boost::asio::io_context::work work(mContext);

		mContextThread = std::thread([&] { mContext.run(); });

		//mSocket.connect(tcp::endpoint(boost::asio::ip::address::from_string(SERVER_ADDRESS.data()), SERVER_PORT), error_code);

		//https://www.reddit.com/r/cpp_questions/comments/15n1r9o/how_does_asioio_contextrun_actually_work/
		//https://www.youtube.com/watch?v=2hNdkYInj4g
		//https://stackoverflow.com/questions/34330576/abort-has-been-called-connect-function-multithreads-cpp

		/*if (error_code)
		{
			printf("Error connecting to server. %s", error_code.message().c_str());
			OutputDebugStringA(error_code.message().c_str());
		}*/
		//mSocket.async_connect()
		//Start to receive data.

		//boost::system::error_code error = boost::asio::error::would_block;
		//std::size_t length = 0;


		mSocket.async_receive(boost::asio::mutable_buffer(mIncomingBuffer->GetBuffer().data(), mIncomingBuffer->GetBufferLength()),
			boost::bind(&Protocol::HandleIncomingData, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

		//run context
		//mContext.run();
	}

	Protocol::~Protocol()
	{
		mContext.stop();

		if (mContextThread.joinable())
			mContextThread.join();
	}

	Bool Protocol::Connect()
	{
		mSocket.async_connect(mEndpoint, [&](boost::system::error_code error_code)
			{
				if (error_code)
				{
					OutputDebugStringA(error_code.message().c_str());
					return false;
				}
				else
				{
					OutputDebugStringA("Connection successful.");
				}
			});

		return true;
	}

	Bool Protocol::Disconnect()
	{
		if (mSocket.is_open())
		{
			//mSocket.cancel(); //TODO: check if needed

			boost::system::error_code error_code;
			mSocket.close(error_code);

			if (error_code)
			{
				OutputDebugStringA(error_code.message().c_str());
				return false;
			}
		}
		return true;
	}

	Bool Protocol::IsConnected()
	{
		return mSocket.is_open(); //TODO: ESTO NO REPRESENTA SI EL SOCKET ESTA CONECTADO O NO, REEMPLAZAR POR UN Bool mIsConnected y trackearlo
	}

	void Protocol::HandleIncomingData(const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		Int32 packetID = mIncomingBuffer->ReadUInt8();

		//OutputDebugStringA("Packet %ld arrived.", packetID);

		switch (static_cast<ServerPacketID>(packetID))
		{
		case ServerPacketID::logged:
			HandleLogged();
			break;
		case ServerPacketID::RemoveDialogs:
			HandleRemoveDialogs();
			break;
		case ServerPacketID::RemoveCharDialog:
			HandleRemoveCharDialog();
			break;
		case ServerPacketID::NavigateToggle:
			HandleNavigateToggle();
			break;
		case ServerPacketID::Disconnect:
			HandleDisconnect();
			break;
		case ServerPacketID::CommerceEnd:
			HandleCommerceEnd();
			break;
		case ServerPacketID::CommerceChat:
			HandleCommerceChat();
			break;
		case ServerPacketID::BankEnd:
			HandleBankEnd();
			break;
		case ServerPacketID::CommerceInit:
			HandleCommerceInit();
			break;
		case ServerPacketID::BankInit:
			HandleBankInit();
			break;
		case ServerPacketID::UserCommerceInit:
			HandleUserCommerceInit();
			break;
		case ServerPacketID::UserCommerceEnd:
			HandleUserCommerceEnd();
			break;
		case ServerPacketID::UserOfferConfirm:
			HandleUserOfferConfirm();
			break;
		case ServerPacketID::ShowBlacksmithForm:
			HandleShowBlacksmithForm();
			break;
		case ServerPacketID::UpdateSta:
			HandleUpdateSta();
			break;
		case ServerPacketID::UpdateMana:
			HandleUpdateMana();
			break;
		case ServerPacketID::UpdateHP:
			HandleUpdateHP();
			break;
		case ServerPacketID::UpdateGold:
			HandleUpdateGold();
			break;
		case ServerPacketID::UpdateBankGold:
			HandleUpdateBankGold();
			break;
		case ServerPacketID::UpdateExp:
			HandleUpdateExp();
			break;
		case ServerPacketID::ChangeMap:
			HandleChangeMap();
			break;
		case ServerPacketID::PosUpdate:
			HandlePosUpdate();
			break;
		case ServerPacketID::ChatOverHead:
			HandleChatOverHead();
			break;
		case ServerPacketID::ConsoleMsg:
			HandleConsoleMessage();
			break;
		case ServerPacketID::GuildChat:
			HandleGuildChat();
			break;
		case ServerPacketID::ShowMessageBox:
			HandleShowMessageBox();
			break;
		case ServerPacketID::UserIndexInServer:
			HandleUserIndexInServer();
			break;
		case ServerPacketID::UserCharIndexInServer:
			HandleUserCharIndexInServer();
			break;
		case ServerPacketID::CharacterCreate:
			HandleCharacterCreate();
			break;
		case ServerPacketID::CharacterRemove:
			HandleCharacterRemove();
			break;
		case ServerPacketID::CharacterChangeNick:
			HandleCharacterChangeNick();
			break;
		case ServerPacketID::CharacterMove:
			HandleCharacterMove();
			break;
		case ServerPacketID::ForceCharMove:
			HandleForceCharMove();
			break;
		case ServerPacketID::CharacterChange:
			HandleCharacterChange();
			break;
		case ServerPacketID::ObjectCreate:
			HandleObjectCreate();
			break;
		case ServerPacketID::ObjectDelete:
			HandleObjectDelete();
			break;
		case ServerPacketID::BlockPosition:
			HandleBlockPosition();
			break;
		case ServerPacketID::PlayMIDI:
			HandlePlayMIDI();
			break;
		case ServerPacketID::PlayWave:
			HandlePlayWave();
			break;
		case ServerPacketID::guildList:
			HandleGuildList();
			break;
		case ServerPacketID::AreaChanged:
			HandleAreaChanged();
			break;
		case ServerPacketID::PauseToggle:
			HandlePauseToggle();
			break;
		case ServerPacketID::RainToggle:
			HandleRainToggle();
			break;
		case ServerPacketID::CreateFX:
			HandleCreateFX();
			break;
		case ServerPacketID::UpdateUserStats:
			HandleUpdateUserStats();
			break;
		case ServerPacketID::WorkRequestTarget:
			HandleWorkRequestTarget();
			break;
		case ServerPacketID::ChangeInventorySlot:
			HandleChangeInventorySlot();
			break;
		case ServerPacketID::ChangeBankSlot:
			HandleChangeBankSlot();
			break;

		case ServerPacketID::ChangeSpellSlot:
			HandleChangeSpellSlot();
			break;

		case ServerPacketID::Atributes:
			HandleAtributes();
			break;

		case ServerPacketID::BlacksmithWeapons:
			HandleBlacksmithWeapons();
			break;

		case ServerPacketID::BlacksmithArmors:
			HandleBlacksmithArmors();
			break;

		case ServerPacketID::CarpenterObjects:
			HandleCarpenterObjects();
			break;

		case ServerPacketID::RestOK:
			HandleRestOK();
			break;

		case ServerPacketID::ErrorMsg:
			HandleErrorMessage();
			break;

		case ServerPacketID::Blind:
			HandleBlind();
			break;

		case ServerPacketID::Dumb:
			HandleDumb();
			break;

		case ServerPacketID::ShowSignal:
			HandleShowSignal();
			break;

		case ServerPacketID::ChangeNPCInventorySlot:
			HandleChangeNPCInventorySlot();
			break;

		case ServerPacketID::UpdateHungerAndThirst:
			HandleUpdateHungerAndThirst();
			break;

		case ServerPacketID::Fame:
			HandleFame();
			break;

		case ServerPacketID::MiniStats:
			HandleMiniStats();
			break;

		case ServerPacketID::LevelUp:
			HandleLevelUp();
			break;

		case ServerPacketID::AddForumMsg:
			HandleAddForumMessage();
			break;

		case ServerPacketID::ShowForumForm:
			HandleShowForumForm();
			break;

		case ServerPacketID::SetInvisible:
			HandleSetInvisible();
			break;

		case ServerPacketID::DiceRoll:
			HandleDiceRoll();
			break;

		case ServerPacketID::MeditateToggle:
			HandleMeditateToggle();
			break;

		case ServerPacketID::BlindNoMore:
			HandleBlindNoMore();
			break;

		case ServerPacketID::DumbNoMore:
			HandleDumbNoMore();
			break;

		case ServerPacketID::SendSkills:
			HandleSendSkills();
			break;

		case ServerPacketID::TrainerCreatureList:
			HandleTrainerCreatureList();
			break;

		case ServerPacketID::guildNews:
			HandleGuildNews();
			break;

		case ServerPacketID::OfferDetails:
			HandleOfferDetails();
			break;

		case ServerPacketID::AlianceProposalsList:
			HandleAlianceProposalsList();
			break;

		case ServerPacketID::PeaceProposalsList:
			HandlePeaceProposalsList();
			break;

		case ServerPacketID::CharacterInfo:
			HandleCharacterInfo();
			break;

		case ServerPacketID::GuildLeaderInfo:
			HandleGuildLeaderInfo();
			break;

		case ServerPacketID::GuildDetails:
			HandleGuildDetails();
			break;

		case ServerPacketID::ShowGuildFundationForm:
			HandleShowGuildFundationForm();
			break;

		case ServerPacketID::ParalizeOK:
			HandleParalizeOK();
			break;

		case ServerPacketID::ShowUserRequest:
			HandleShowUserRequest();
			break;

		case ServerPacketID::TradeOK:
			HandleTradeOK();
			break;

		case ServerPacketID::BankOK:
			HandleBankOK();
			break;

		case ServerPacketID::ChangeUserTradeSlot:
			HandleChangeUserTradeSlot();
			break;

		case ServerPacketID::SendNight:
			HandleSendNight();
			break;

		case ServerPacketID::Pong:
			HandlePong();
			break;

		case ServerPacketID::UpdateTagAndStatus:
			HandleUpdateTagAndStatus();
			break;

		case ServerPacketID::GuildMemberInfo:
			HandleGuildMemberInfo();
			break;

		case ServerPacketID::SpawnList:
			HandleSpawnList();
			break;

		case ServerPacketID::ShowSOSForm:
			HandleShowSOSForm();
			break;

		case ServerPacketID::ShowDenounces:
			HandleShowDenounces();
			break;

		case ServerPacketID::RecordDetails:
			HandleRecordDetails();
			break;

		case ServerPacketID::RecordList:
			HandleRecordList();
			break;

		case ServerPacketID::ShowMOTDEditionForm:
			HandleShowMOTDEditionForm();
			break;

		case ServerPacketID::ShowGMPanelForm:
			HandleShowGMPanelForm();
			break;

		case ServerPacketID::UserNameList:
			HandleUserNameList();
			break;

		case ServerPacketID::ShowGuildAlign:
			HandleShowGuildAlign();
			break;

		case ServerPacketID::ShowPartyForm:
			HandleShowPartyForm();
			break;

		case ServerPacketID::UpdateStrenghtAndDexterity:
			HandleUpdateStrenghtAndDexterity();
			break;

		case ServerPacketID::UpdateStrenght:
			HandleUpdateStrenght();
			break;

		case ServerPacketID::UpdateDexterity:
			HandleUpdateDexterity();
			break;

		case ServerPacketID::AddSlots:
			HandleAddSlots();
			break;

		case ServerPacketID::MultiMessage:
			HandleMultiMessage();
			break;

		case ServerPacketID::StopWorking:
			HandleStopWorking();
			break;

		case ServerPacketID::CancelOfferItem:
			HandleCancelOfferItem();
			break;
		}

		//TODO: check if there is more data to parse
	}


	void Protocol::SendData()
	{

		if (mOutgoingBuffer->GetBufferPosition() == 0) return;

		// TODO: cant guarantee that internal buffer is going to hold the same data until the actual send occurs?
		// Lock a portion of the buffer before flipping and free it in the callback?
		//mOutgoingBuffer->Flip();
		//mSocket.async_send(boost::asio::buffer(mOutgoingBuffer->GetBuffer()))

		mSocket.send(boost::asio::buffer(mOutgoingBuffer->GetBuffer(), mOutgoingBuffer->GetBufferPosition()));
		mOutgoingBuffer->Rewind();
	}


	void Protocol::HandleMultiMessage()
	{
		//***************************************************
		//Author: Unknown
		//Last Modification: 11/16/2010
		// 09/28/2010: C4b3z0n - Ahora se le saco la "," a los minutos de distancia del /hogar, ya que a veces quedaba "12,5 minutos y 30segundos"
		// 09/21/2010: C4b3z0n - Now the fragshooter operates taking the screen after the change of killed charindex to ghost only if target charindex is visible to the client, else it will take screenshot like before.
		// 11/16/2010: Amraphen - Recoded how the FragShooter works.
		//***************************************************
		UInt8 bodyPart;
		Int32 dmg;


		switch (mIncomingBuffer->ReadUInt8())
		{
			/*
			Case eMessages.DontSeeAnything
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_NO_VES_NADA_INTERESANTE, 65, 190, 156, False, False, True)

				Case eMessages.NPCSwing
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_CRIATURA_FALLA_GOLPE, 255, 0, 0, True, False, True)

				Case eMessages.NPCKillUser
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_CRIATURA_MATADO, 255, 0, 0, True, False, True)

				Case eMessages.BlockedWithShieldUser
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_RECHAZO_ATAQUE_ESCUDO, 255, 0, 0, True, False, True)

				Case eMessages.BlockedWithShieldOther
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_USUARIO_RECHAZO_ATAQUE_ESCUDO, 255, 0, 0, True, False, True)

				Case eMessages.UserSwing
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_FALLADO_GOLPE, 255, 0, 0, True, False, True)

				Case eMessages.SafeModeOn
				frmMain.ControlSM(eSMType.sSafemode, True)

				Case eMessages.SafeModeOff
				frmMain.ControlSM(eSMType.sSafemode, False)

				Case eMessages.ResuscitationSafeOff
				frmMain.ControlSM(eSMType.sResucitation, False)

				Case eMessages.ResuscitationSafeOn
				frmMain.ControlSM(eSMType.sResucitation, True)

				Case eMessages.NobilityLost
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PIERDE_NOBLEZA, 255, 0, 0, False, False, True)

				Case eMessages.CantUseWhileMeditating
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_USAR_MEDITANDO, 255, 0, 0, False, False, True)

				Case eMessages.NPCHitUser
				Select Case incomingData.ReadByte()
				Case bCabeza
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_CABEZA & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)

				Case bBrazoIzquierdo
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_BRAZO_IZQ & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)

				Case bBrazoDerecho
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_BRAZO_DER & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)

				Case bPiernaIzquierda
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_PIERNA_IZQ & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)

				Case bPiernaDerecha
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_PIERNA_DER & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)

				Case bTorso
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_TORSO & CStr(incomingData.ReadInteger() & "!!"), 255, 0, 0, True, False, True)
				End Select

				Case eMessages.UserHitNPC
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_CRIATURA_1 & CStr(incomingData.ReadLong()) & MENSAJE_2, 255, 0, 0, True, False, True)

				Case eMessages.UserAttackedSwing
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & charlist(incomingData.ReadInteger()).Nombre & MENSAJE_ATAQUE_FALLO, 255, 0, 0, True, False, True)

				Case eMessages.UserHittedByUser
				DimString AttackerName

				AttackerName = GetRawName(charlist(incomingData.ReadInteger()).Nombre)
				BodyPart = incomingData.ReadByte()
				Daño = incomingData.ReadInteger()

				Select Case BodyPart
				Case bCabeza
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & AttackerName & MENSAJE_RECIVE_IMPACTO_CABEZA & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bBrazoIzquierdo
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & AttackerName & MENSAJE_RECIVE_IMPACTO_BRAZO_IZQ & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bBrazoDerecho
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & AttackerName & MENSAJE_RECIVE_IMPACTO_BRAZO_DER & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bPiernaIzquierda
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & AttackerName & MENSAJE_RECIVE_IMPACTO_PIERNA_IZQ & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bPiernaDerecha
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & AttackerName & MENSAJE_RECIVE_IMPACTO_PIERNA_DER & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bTorso
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & AttackerName & MENSAJE_RECIVE_IMPACTO_TORSO & Daño & MENSAJE_2, 255, 0, 0, True, False, True)
				End Select

				Case eMessages.UserHittedUser

				DimString VictimName

				VictimName = GetRawName(charlist(incomingData.ReadInteger()).Nombre)
				BodyPart = incomingData.ReadByte()
				Daño = incomingData.ReadInteger()

				Select Case BodyPart
				Case bCabeza
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & VictimName & MENSAJE_PRODUCE_IMPACTO_CABEZA & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bBrazoIzquierdo
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & VictimName & MENSAJE_PRODUCE_IMPACTO_BRAZO_IZQ & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bBrazoDerecho
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & VictimName & MENSAJE_PRODUCE_IMPACTO_BRAZO_DER & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bPiernaIzquierda
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & VictimName & MENSAJE_PRODUCE_IMPACTO_PIERNA_IZQ & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bPiernaDerecha
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & VictimName & MENSAJE_PRODUCE_IMPACTO_PIERNA_DER & Daño & MENSAJE_2, 255, 0, 0, True, False, True)

				Case bTorso
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & VictimName & MENSAJE_PRODUCE_IMPACTO_TORSO & Daño & MENSAJE_2, 255, 0, 0, True, False, True)
				End Select

				Case eMessages.WorkRequestTarget
				UsingSkill = incomingData.ReadByte()

				frmMain.MousePointer = 2

				Select Case UsingSkill
				Case Magia
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_MAGIA, 100, 100, 120, 0, 0)

				Case Pesca
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_PESCA, 100, 100, 120, 0, 0)

				Case Robar
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_ROBAR, 100, 100, 120, 0, 0)

				Case Talar
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_TALAR, 100, 100, 120, 0, 0)

				Case Mineria
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_MINERIA, 100, 100, 120, 0, 0)

				Case FundirMetal
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_FUNDIRMETAL, 100, 100, 120, 0, 0)

				Case Proyectiles
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_PROYECTILES, 100, 100, 120, 0, 0)
				End Select

				Case eMessages.HaveKilledUser
				DimInt32 KilledUser
				DimInt32 Exp

				KilledUser = .ReadInteger
				Exp = .ReadLong

				ShowConsoleMsg(MENSAJE_HAS_MATADO_A & charlist(KilledUser).Nombre & MENSAJE_22, 255, 0, 0, True, False)
				ShowConsoleMsg(MENSAJE_HAS_GANADO_EXPE_1 & Exp & MENSAJE_HAS_GANADO_EXPE_2, 255, 0, 0, True, False)

				//Sacamos un screenshot si está activado el FragShooter:
				If ClientSetup.bKill And ClientSetup.bActive Then
				If Exp \ 2 > ClientSetup.byMurderedLevel Then
				FragShooterNickname = charlist(KilledUser).Nombre
				FragShooterKilledSomeone = True

				FragShooterCapturePending = True
				End If
				End If

				Case eMessages.UserKill
				DimInt32 KillerUser

				KillerUser = .ReadInteger

				ShowConsoleMsg(charlist(KillerUser).Nombre & MENSAJE_TE_HA_MATADO, 255, 0, 0, True, False)

				//Sacamos un screenshot si está activado el FragShooter:
				If ClientSetup.bDie And ClientSetup.bActive Then
				FragShooterNickname = charlist(KillerUser).Nombre
				FragShooterKilledSomeone = False

				FragShooterCapturePending = True
				End If

				Case eMessages.EarnExp
				//ShowConsoleMsg(MENSAJE_HAS_GANADO_EXPE_1 & .ReadLong & MENSAJE_HAS_GANADO_EXPE_2, 255, 0, 0, True, False)

				Case eMessages.GoHome
				DimUInt8 Distance
				DimString Hogar
				DimInt32 tiempo
				DimString msg

				Distance = .ReadByte
				tiempo = .ReadInteger
				Hogar = .ReadASCIIString

				If tiempo >= 60 Then
				If tiempo Mod 60 = 0 Then
				msg = tiempo / 60 & " minutos."
				Else
				msg = CInt(tiempo \ 60) & " minutos y " & tiempo Mod 60 & " segundos."  //Agregado el CInt() asi el número no es con , [C4b3z0n - 09/28/2010]
				End If
				Else
				msg = tiempo & " segundos."
				End If

				ShowConsoleMsg("Te encuentras a " & Distance & " mapas de la " & Hogar & ", este viaje durará " & msg, 255, 0, 0, True)
				Traveling = True

				Case eMessages.FinishHome
				ShowConsoleMsg(MENSAJE_HOGAR, 255, 255, 255)
				Traveling = False

				Case eMessages.CancelGoHome
				ShowConsoleMsg(MENSAJE_HOGAR_CANCEL, 255, 0, 0, True)
				Traveling = False
				*/
		}
	}

	//
	// Handles the Logged message.

	void Protocol::HandleLogged()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			// Variable initialization
		mIncomingBuffer->ReadUInt8();//UserClase = mIncomingBuffer->ReadUInt8();
		//EngineRun = True
		//Nombres = True
		//bRain = False

		//Set connected state
		//SetConnected

		//If bShowTutorial Then frmTutorial.Show vbModeless

		//Show tip
		//If tipf = "1" And PrimeraVez Then
		//CargarTip
		//frmtip.Visible = True
		//PrimeraVez = False
		//End If
	}

	//
	// Handles the RemoveDialogs message.

	void Protocol::HandleRemoveDialogs()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		//Dialogos.RemoveAllDialogs
	}

	//
	// Handles the RemoveCharDialog message.

	void Protocol::HandleRemoveCharDialog()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		mIncomingBuffer->ReadInt16();//Dialogos.RemoveDialog(incomingData.ReadInteger())
	}

	//
	// Handles the NavigateToggle message.

	void Protocol::HandleNavigateToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		//UserNavegando = Not UserNavegando
	}

	//
	// Handles the Disconnect message.

	void Protocol::HandleDisconnect()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//Close connection
			//Close socket
			//ResetAllInfo
	}

	//
	// Handles the CommerceEnd message.

	void Protocol::HandleCommerceEnd()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		//Set InvComUsu = Nothing
		//Set InvComNpc = Nothing

		//Hide form
		//Unload frmComerciar

		//Reset vars
		//Comerciando = False
	}

	//
	// Handles the BankEnd message.

	void Protocol::HandleBankEnd()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		/*
		Set InvBanco(0) = Nothing
		Set InvBanco(1) = Nothing

		Unload frmBancoObj
		Comerciando = False*/
	}

	//
	// Handles the CommerceInit message.

	void Protocol::HandleCommerceInit()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


			//Set InvComUsu = New clsGrapchicalInventory
			//Set InvComNpc = New clsGrapchicalInventory

			// Initialize commerce inventories
			//InvComUsu.Initialize(DirectDraw, frmComerciar.picInvUser, Inventario.MaxObjs)
			//InvComNpc.Initialize(DirectDraw, frmComerciar.picInvNpc, MAX_NPC_INVENTORY_SLOTS)

			//Fill user inventory
		   /* For i = 1 To MAX_INVENTORY_SLOTS
			If Inventario.OBJIndex(i) < > 0 Then
			With Inventario
			InvComUsu.SetItem(i, .OBJIndex(i), _
				.Amount(i), .Equipped(i), .GrhIndex(i), _
				.OBJType(i), .MaxHit(i), .MinHit(i), .MaxDef(i), .MinDef(i), _
				.Valor(i), .ItemName(i))

			End If
			Next i

			// Fill Npc inventory
			For i = 1 To 50
			If NPCInventory(i).OBJIndex < > 0 Then
			With NPCInventory(i)
			InvComNpc.SetItem(i, .OBJIndex, _
				.Amount, 0, .GrhIndex, _
				.OBJType, .MaxHit, .MinHit, .MaxDef, .MinDef, _
				.Valor, .Name)

			End If
			Next i

			//Set state and show form
			Comerciando = True
			frmComerciar.Show, frmMain*/
	}

	//
	// Handles the BankInit message.

	void Protocol::HandleBankInit()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
		int bankGold = mIncomingBuffer->ReadInt32();


		/* Set InvBanco(0) = New clsGrapchicalInventory
		 Set InvBanco(1) = New clsGrapchicalInventory

		 BankGold = incomingData.ReadLong
		 InvBanco(0).Initialize(DirectDraw, frmBancoObj.PicBancoInv, MAX_BANCOINVENTORY_SLOTS)
		 InvBanco(1).Initialize(DirectDraw, frmBancoObj.PicInv, Inventario.MaxObjs)

		 For i = 1 To Inventario.MaxObjs
		 With Inventario
		 InvBanco(1).SetItem(i, .OBJIndex(i), _
			 .Amount(i), .Equipped(i), .GrhIndex(i), _
			 .OBJType(i), .MaxHit(i), .MinHit(i), .MaxDef(i), .MinDef(i), _
			 .Valor(i), .ItemName(i))

		 Next i

		 For i = 1 To MAX_BANCOINVENTORY_SLOTS
		 With UserBancoInventory(i)
		 InvBanco(0).SetItem(i, .OBJIndex, _
			 .Amount, .Equipped, .GrhIndex, _
			 .OBJType, .MaxHit, .MinHit, .MaxDef, .MinDef, _
			 .Valor, .Name)

		 Next i

		 //Set state and show form
		 Comerciando = True

		 frmBancoObj.lblUserGld.Caption = BankGold

		 frmBancoObj.Show, frmMain*/
	}

	//
	// Handles the UserCommerceInit message.

	void Protocol::HandleUserCommerceInit()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			/*TradingUserName = incomingData.ReadASCIIString

			Set InvComUsu = New clsGrapchicalInventory
			Set InvOfferComUsu(0) = New clsGrapchicalInventory
			Set InvOfferComUsu(1) = New clsGrapchicalInventory
			Set InvOroComUsu(0) = New clsGrapchicalInventory
			Set InvOroComUsu(1) = New clsGrapchicalInventory
			Set InvOroComUsu(2) = New clsGrapchicalInventory

			// Initialize commerce inventories
			InvComUsu.Initialize(DirectDraw, frmComerciarUsu.picInvComercio, Inventario.MaxObjs)
			InvOfferComUsu(0).Initialize(DirectDraw, frmComerciarUsu.picInvOfertaProp, INV_OFFER_SLOTS)
			InvOfferComUsu(1).Initialize(DirectDraw, frmComerciarUsu.picInvOfertaOtro, INV_OFFER_SLOTS)
			InvOroComUsu(0).Initialize(DirectDraw, frmComerciarUsu.picInvOroProp, INV_GOLD_SLOTS, , TilePixelWidth * 2, TilePixelHeight, TilePixelWidth / 2, , , , True)
			InvOroComUsu(1).Initialize(DirectDraw, frmComerciarUsu.picInvOroOfertaProp, INV_GOLD_SLOTS, , TilePixelWidth * 2, TilePixelHeight, TilePixelWidth / 2, , , , True)
			InvOroComUsu(2).Initialize(DirectDraw, frmComerciarUsu.picInvOroOfertaOtro, INV_GOLD_SLOTS, , TilePixelWidth * 2, TilePixelHeight, TilePixelWidth / 2, , , , True)

			//Fill user inventory
			For i = 1 To MAX_INVENTORY_SLOTS
			If Inventario.OBJIndex(i) < > 0 Then
			With Inventario
			InvComUsu.SetItem(i, .OBJIndex(i), _
				.Amount(i), .Equipped(i), .GrhIndex(i), _
				.OBJType(i), .MaxHit(i), .MinHit(i), .MaxDef(i), .MinDef(i), _
				.Valor(i), .ItemName(i))

			End If
			Next i

			// Inventarios de oro
			InvOroComUsu(0).SetItem(1, ORO_INDEX, UserGLD, 0, ORO_GRH, 0, 0, 0, 0, 0, 0, "Oro")
			InvOroComUsu(1).SetItem(1, ORO_INDEX, 0, 0, ORO_GRH, 0, 0, 0, 0, 0, 0, "Oro")
			InvOroComUsu(2).SetItem(1, ORO_INDEX, 0, 0, ORO_GRH, 0, 0, 0, 0, 0, 0, "Oro")


			//Set state and show form
			Comerciando = True
			frmComerciarUsu.Show(vbModeless, frmMain)*/
	}

	//
	// Handles the UserCommerceEnd message.

	void Protocol::HandleUserCommerceEnd()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			/*Set InvComUsu = Nothing
			Set InvOroComUsu(0) = Nothing
			Set InvOroComUsu(1) = Nothing
			Set InvOroComUsu(2) = Nothing
			Set InvOfferComUsu(0) = Nothing
			Set InvOfferComUsu(1) = Nothing

			//Destroy the form and reset the state
			Unload frmComerciarUsu
			Comerciando = False*/
	}

	//
	// Handles the UserOfferConfirm message.
	void Protocol::HandleUserOfferConfirm()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 14/12/2009
		//
		//***************************************************
		/*
			With frmComerciarUsu
			// Now he can accept the offer or reject it
			.HabilitarAceptarRechazar True

			.PrintCommerceMsg TradingUserName & " ha confirmado su oferta!", FontTypeNames.FONTTYPE_CONSE

		*/
	}

	//
	// Handles the ShowBlacksmithForm message.

	void Protocol::HandleShowBlacksmithForm()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
		/*
			If frmMain.macrotrabajo.Enabled And(MacroBltIndex > 0) Then
			WriteCraftBlacksmith(MacroBltIndex)
			Else
			frmHerrero.Show, frmMain
			MirandoHerreria = True
			End If
		*/
	}

	//
	// Handles the ShowCarpenterForm message.

	void Protocol::HandleShowCarpenterForm()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
		/*

			If frmMain.macrotrabajo.Enabled And(MacroBltIndex > 0) Then
			WriteCraftCarpenter(MacroBltIndex)
			Else
			frmCarp.Show, frmMain
			MirandoCarpinteria = True
			End If
			*/
	}

	//
	// Handles the NPCSwing message.

	void Protocol::HandleNPCSwing()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		//AddtoRichTextBox(frmMain.RecTxt, MENSAJE_CRIATURA_FALLA_GOLPE, 255, 0, 0, True, False, True)
	}

	//
	// Handles the NPCKillUser message.

	void Protocol::HandleNPCKillUser()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		   //AddtoRichTextBox(frmMain.RecTxt, MENSAJE_CRIATURA_MATADO, 255, 0, 0, True, False, True)
	}

	//
	// Handles the BlockedWithShieldUser message.

	void Protocol::HandleBlockedWithShieldUser()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		//AddtoRichTextBox(frmMain.RecTxt, MENSAJE_RECHAZO_ATAQUE_ESCUDO, 255, 0, 0, True, False, True)
	}

	//
	// Handles the BlockedWithShieldOther message.

	void Protocol::HandleBlockedWithShieldOther()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


			//AddtoRichTextBox(frmMain.RecTxt, MENSAJE_USUARIO_RECHAZO_ATAQUE_ESCUDO, 255, 0, 0, True, False, True)
	}

	//
	// Handles the UserSwing message.

	void Protocol::HandleUserSwing()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//AddtoRichTextBox(frmMain.RecTxt, MENSAJE_FALLADO_GOLPE, 255, 0, 0, True, False, True)
	}

	//
	// Handles the SafeModeOn message.

	void Protocol::HandleSafeModeOn()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		   // frmMain.ControlSM(eSMType.sSafemode, True)
	}

	//
	// Handles the SafeModeOff message.

	void Protocol::HandleSafeModeOff()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		//frmMain.ControlSM(eSMType.sSafemode, False)
	}

	//
	// Handles the ResuscitationSafeOff message.

	void Protocol::HandleResuscitationSafeOff()
	{
		//***************************************************
		//Author: Rapsodius
		//Creation date: 10/10/07
		//***************************************************


			//frmMain.ControlSM(eSMType.sResucitation, False)
	}

	//
	// Handles the ResuscitationSafeOn message.

	void Protocol::HandleResuscitationSafeOn()
	{
		//***************************************************
		//Author: Rapsodius
		//Creation date: 10/10/07
		//***************************************************


			//frmMain.ControlSM(eSMType.sResucitation, True)
	}

	//
	// Handles the NobilityLost message.

	void Protocol::HandleNobilityLost()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		   // AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PIERDE_NOBLEZA, 255, 0, 0, False, False, True)
	}

	//
	// Handles the CantUseWhileMeditating message.

	void Protocol::HandleCantUseWhileMeditating()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		   // AddtoRichTextBox(frmMain.RecTxt, MENSAJE_USAR_MEDITANDO, 255, 0, 0, False, False, True)
	}

	//
	// Handles the UpdateSta message.

	void Protocol::HandleUpdateSta()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


			//Get data and update form
		mIncomingBuffer->ReadInt16();//UserMinSTA = incomingData.ReadInteger()

		/* frmMain.lblEnergia = UserMinSTA & "/" & UserMaxSTA

		 DimUInt8 bWidth

		 bWidth = (((UserMinSTA / 100) / (UserMaxSTA / 100)) * 75)

		 frmMain.shpEnergia.Width = 75 - bWidth
		 frmMain.shpEnergia.Left = 584 + (75 - frmMain.shpEnergia.Width)

		 frmMain.shpEnergia.Visible = (bWidth < > 75)*/

	}

	//
	// Handles the UpdateMana message.

	void Protocol::HandleUpdateMana()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


			//Get data and update form
		mIncomingBuffer->ReadInt16();//UserMinMAN = incomingData.ReadInteger()
		/*
			frmMain.lblMana = UserMinMAN & "/" & UserMaxMAN

			DimUInt8 bWidth

			If UserMaxMAN > 0 Then _
			bWidth = (((UserMinMAN / 100) / (UserMaxMAN / 100)) * 75)

			frmMain.shpMana.Width = 75 - bWidth
			frmMain.shpMana.Left = 584 + (75 - frmMain.shpMana.Width)

			frmMain.shpMana.Visible = (bWidth < > 75)*/
	}

	//
	// Handles the UpdateHP message.

	void Protocol::HandleUpdateHP()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//Get data and update form
		mIncomingBuffer->ReadInt16();//UserMinHP = incomingData.ReadInteger()

		/*frmMain.lblVida = UserMinHP & "/" & UserMaxHP

		DimUInt8 bWidth

		bWidth = (((UserMinHP / 100) / (UserMaxHP / 100)) * 75)

		frmMain.shpVida.Width = 75 - bWidth
		frmMain.shpVida.Left = 584 + (75 - frmMain.shpVida.Width)

		frmMain.shpVida.Visible = (bWidth < > 75)

		//Is the user alive??
		If UserMinHP = 0 Then
		UserEstado = 1
		If frmMain.TrainingMacro Then frmMain.DesactivarMacroHechizos
		If frmMain.macrotrabajo Then frmMain.DesactivarMacroTrabajo
		Else
		UserEstado = 0
		End If*/
	}

	//
	// Handles the UpdateGold message.

	void Protocol::HandleUpdateGold()
	{
		//***************************************************
		//Autor: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 09/21/10
		//Last Modified By: C4b3z0n
		//- 08/14/07: Tavo - Added GldLbl color variation depending on User Gold and Level
		//- 09/21/10: C4b3z0n - Modified color change of gold ONLY if the player//s level is greater than 12 (NOT newbie).
		//***************************************************

			//Get data and update form
		mIncomingBuffer->ReadInt32();//UserGLD = incomingData.ReadLong()

		/*If UserGLD >= CLng(UserLvl) * 10000 And UserLvl > 12 Then //Si el nivel es mayor de 12, es decir, no es newbie.
		//Changes color
		frmMain.GldLbl.ForeColor = &HFF & //Red
		Else
		//Changes color
		frmMain.GldLbl.ForeColor = &HFFFF & //Yellow
		End If

		frmMain.GldLbl.Caption = UserGLD*/
	}

	//
	// Handles the UpdateBankGold message.

	void Protocol::HandleUpdateBankGold()
	{
		//***************************************************
		//Autor: ZaMa
		//Last Modification: 14/12/2009
		//
		//***************************************************


		mIncomingBuffer->ReadInt32();//frmBancoObj.lblUserGld.Caption = incomingData.ReadLong

	}

	//
	// Handles the UpdateExp message.

	void Protocol::HandleUpdateExp()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//Get data and update form

		mIncomingBuffer->ReadInt32();//UserExp = incomingData.ReadLong()
		//frmMain.lblExp.Caption = "Exp: " & UserExp & "/" & UserPasarNivel
		//frmMain.lblPorcLvl.Caption = "[" & Round(CDbl(UserExp) * CDbl(100) / CDbl(UserPasarNivel), 2) & "%]"
	}

	//
	// Handles the UpdateStrenghtAndDexterity message.

	void Protocol::HandleUpdateStrenghtAndDexterity()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 11/26/09
		//***************************************************


			//Get data and update form
		mIncomingBuffer->ReadUInt8();//UserFuerza = incomingData.ReadByte
		mIncomingBuffer->ReadUInt8();//UserAgilidad = incomingData.ReadByte
		//frmMain.lblStrg.Caption = UserFuerza
		//frmMain.lblDext.Caption = UserAgilidad
		//frmMain.lblStrg.ForeColor = getStrenghtColor()
		//frmMain.lblDext.ForeColor = getDexterityColor()
	}

	// Handles the UpdateStrenghtAndDexterity message.

	void Protocol::HandleUpdateStrenght()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 11/26/09
		//***************************************************

			//Get data and update form
		mIncomingBuffer->ReadUInt8();//UserFuerza = incomingData.ReadByte
		//frmMain.lblStrg.Caption = UserFuerza
		//frmMain.lblStrg.ForeColor = getStrenghtColor()
	}

	// Handles the UpdateStrenghtAndDexterity message.

	void Protocol::HandleUpdateDexterity()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 11/26/09
		//***************************************************

			//Get data and update form
		mIncomingBuffer->ReadUInt8();//UserAgilidad = incomingData.ReadByte
		//frmMain.lblDext.Caption = UserAgilidad
		//frmMain.lblDext.ForeColor = getDexterityColor()
	}

	//
	// Handles the ChangeMap message.
	void Protocol::HandleChangeMap()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadInt16();//UserMap = incomingData.ReadInteger()

		//TODO: Once on-the-fly editor is implemented check for map version before loading....
		//For now we just drop it
		mIncomingBuffer->ReadInt16();
		/*
			If FileExist(DirMapas & "Mapa" & UserMap & ".map", vbNormal) Then
			SwitchMap(UserMap)
			If bLluvia(UserMap) = 0 Then
			If bRain Then
			Audio.StopWave(RainBufferIndex)
			RainBufferIndex = 0
			frmMain.IsPlaying = PlayLoop.plNone
			End If
			End If
			Else
			//no encontramos el mapa en el hd
			MsgBox "Error en los mapas, algún archivo ha sido modificado o esta dañado."

			CloseClient
			End If*/
	}

	//
	// Handles the PosUpdate message.

	void Protocol::HandlePosUpdate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//Remove char from old position
		   /* If MapData(UserPos.X, UserPos.Y).CharIndex = UserCharIndex Then
			MapData(UserPos.X, UserPos.Y).CharIndex = 0
			End If*/

			//Set new pos
		mIncomingBuffer->ReadUInt8();//UserPos.X = incomingData.ReadByte()
		mIncomingBuffer->ReadUInt8();//UserPos.Y = incomingData.ReadByte()

		//Set char
		//MapData(UserPos.X, UserPos.Y).CharIndex = UserCharIndex
		//charlist(UserCharIndex).Pos = UserPos

		//Are we under a roof?
		/*bTecho = IIf(MapData(UserPos.X, UserPos.Y).Trigger = 1 Or _
			MapData(UserPos.X, UserPos.Y).Trigger = 2 Or _
			MapData(UserPos.X, UserPos.Y).Trigger = 4, True, False)

		//Update pos label
		frmMain.Coord.Caption = UserMap & " X: " & UserPos.X & " Y: " & UserPos.Y*/
	}

	//
	// Handles the NPCHitUser message.

	void Protocol::HandleNPCHitUser()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		switch (mIncomingBuffer->ReadUInt8())
		{
			mIncomingBuffer->ReadInt16();//tmp, sacar

			/*case bCabeza
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_CABEZA & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)
				Case bBrazoIzquierdo
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_BRAZO_IZQ & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)
				Case bBrazoDerecho
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_BRAZO_DER & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)
				Case bPiernaIzquierda
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_PIERNA_IZQ & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)
				Case bPiernaDerecha
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_PIERNA_DER & CStr(incomingData.ReadInteger()) & "!!", 255, 0, 0, True, False, True)
				Case bTorso
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_TORSO & CStr(incomingData.ReadInteger() & "!!"), 255, 0, 0, True, False, True)
				*/
		}
	}

	//
	// Handles the UserHitNPC message.

	void Protocol::HandleUserHitNPC()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadInt32();//tmp, sacar
		//AddtoRichTextBox(frmMain.RecTxt, MENSAJE_GOLPE_CRIATURA_1 & CStr(incomingData.ReadLong()) & MENSAJE_2, 255, 0, 0, True, False, True)
	}

	//
	// Handles the UserAttackedSwing message.

	void Protocol::HandleUserAttackedSwing()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadInt16();//tmp sacar
		//AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & charlist(incomingData.ReadInteger()).Nombre & MENSAJE_ATAQUE_FALLO, 255, 0, 0, True, False, True)
	}

	//
	// Handles the UserHittingByUser message.

	void Protocol::HandleUserHittedByUser()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadInt16();//tmp sacar
		String attacker = "";// charlist(incomingData.ReadInteger()).Nombre

		switch (mIncomingBuffer->ReadUInt8())
		{
			/*Case bCabeza
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & attacker & MENSAJE_RECIVE_IMPACTO_CABEZA & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bBrazoIzquierdo
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & attacker & MENSAJE_RECIVE_IMPACTO_BRAZO_IZQ & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bBrazoDerecho
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & attacker & MENSAJE_RECIVE_IMPACTO_BRAZO_DER & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bPiernaIzquierda
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & attacker & MENSAJE_RECIVE_IMPACTO_PIERNA_IZQ & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bPiernaDerecha
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & attacker & MENSAJE_RECIVE_IMPACTO_PIERNA_DER & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bTorso
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_1 & attacker & MENSAJE_RECIVE_IMPACTO_TORSO & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				End Select
				*/
		}

	}

	//
	// Handles the UserHittedUser message.

	void Protocol::HandleUserHittedUser()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadInt16();//tmp sacar
		String victim = "";// charlist(incomingData.ReadInteger()).Nombre

		switch (mIncomingBuffer->ReadUInt8())
		{
			/*Case bCabeza
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & Victim & MENSAJE_PRODUCE_IMPACTO_CABEZA & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bBrazoIzquierdo
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & Victim & MENSAJE_PRODUCE_IMPACTO_BRAZO_IZQ & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bBrazoDerecho
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & Victim & MENSAJE_PRODUCE_IMPACTO_BRAZO_DER & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bPiernaIzquierda
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & Victim & MENSAJE_PRODUCE_IMPACTO_PIERNA_IZQ & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bPiernaDerecha
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & Victim & MENSAJE_PRODUCE_IMPACTO_PIERNA_DER & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				Case bTorso
				AddtoRichTextBox(frmMain.RecTxt, MENSAJE_PRODUCE_IMPACTO_1 & Victim & MENSAJE_PRODUCE_IMPACTO_TORSO & CStr(incomingData.ReadInteger() & MENSAJE_2), 255, 0, 0, True, False, True)
				End Select*/
		}
	}

	//
	// Handles the ChatOverHead message.

	void Protocol::HandleChatOverHead()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		String chat = ""; //TODO: readASCIISTRING
		Int32 charIndex = mIncomingBuffer->ReadInt16();

		UInt8 r = mIncomingBuffer->ReadUInt8();
		UInt8 g = mIncomingBuffer->ReadUInt8();
		UInt8 b = mIncomingBuffer->ReadUInt8();

		//Only add the chat if the character exists (a CharacterRemove may have been sent to the PC / NPC area before the buffer was flushed)
		//If charlist(CharIndex).Active Then _
		//Dialogos.CreateDialog(Trim$(chat), CharIndex, RGB(r, g, b))

	}

	//
	// Handles the ConsoleMessage message.

	void Protocol::HandleConsoleMessage()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		String chat = ""; //TODO: readstring
		Int32 fontIndex = mIncomingBuffer->ReadUInt8();
		String str = "";
		UInt8 r = 0;
		UInt8 g = 0;
		UInt8 b = 0;
		/*
		If InStr(1, chat, "~") Then
		str = ReadField(2, chat, 126)
		If Val(str) > 255 Then
		r = 255
		Else
		r = Val(str)
		End If

		str = ReadField(3, chat, 126)
		If Val(str) > 255 Then
		g = 255
		Else
		g = Val(str)
		End If

		str = ReadField(4, chat, 126)
		If Val(str) > 255 Then
		b = 255
		Else
		b = Val(str)
		End If

		AddtoRichTextBox(frmMain.RecTxt, Left$(chat, InStr(1, chat, "~") - 1), r, g, b, Val(ReadField(5, chat, 126)) < > 0, Val(ReadField(6, chat, 126)) < > 0)
		Else
		With FontTypes(FontIndex)
		AddtoRichTextBox(frmMain.RecTxt, chat, .red, .green, .blue, .bold, .italic)


		// Para no perder el foco cuando chatea por party
		If FontIndex = FontTypeNames.FONTTYPE_PARTY Then
		If MirandoParty Then frmParty.SendTxt.SetFocus
		End If
		End If*/

	}

	//
	// Handles the GuildChat message.

	void Protocol::HandleGuildChat()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 04/07/08 (NicoNZ)
		//
		//***************************************************

		String chat = ""; //TOOD readstring
		String str = "";
		Int32 r = 0;
		Int32 g = 0;
		Int32 b = 0;
		Int32 tmp = 0;
		Int32 Cont = 0;

		/*
			chat = Buffer.ReadASCIIString()

			If Not DialogosClanes.Activo Then
			If InStr(1, chat, "~") Then
			str = ReadField(2, chat, 126)
			If Val(str) > 255 Then
			r = 255
			Else
			r = Val(str)
			End If

			str = ReadField(3, chat, 126)
			If Val(str) > 255 Then
			g = 255
			Else
			g = Val(str)
			End If

			str = ReadField(4, chat, 126)
			If Val(str) > 255 Then
			b = 255
			Else
			b = Val(str)
			End If

			AddtoRichTextBox(frmMain.RecTxt, Left$(chat, InStr(1, chat, "~") - 1), r, g, b, Val(ReadField(5, chat, 126)) < > 0, Val(ReadField(6, chat, 126)) < > 0)
			Else
			With FontTypes(FontTypeNames.FONTTYPE_GUILDMSG)
			AddtoRichTextBox(frmMain.RecTxt, chat, .red, .green, .blue, .bold, .italic)

			End If
			Else
			DialogosClanes.PushBackText(ReadField(1, chat, 126))
			End If

			*/
	}

	//
	// Handles the ConsoleMessage message.

	void Protocol::HandleCommerceChat()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 03/12/2009
		//
		//***************************************************

		String chat = ""; //TODO: readstring
		Int32 fontIndex = mIncomingBuffer->ReadUInt8();
		String str = "";
		UInt8 r = 0;
		UInt8 g = 0;
		UInt8 b = 0;

		/*
					If InStr(1, chat, "~") Then
					str = ReadField(2, chat, 126)
					If Val(str) > 255 Then
					r = 255
					Else
					r = Val(str)
					End If

					str = ReadField(3, chat, 126)
					If Val(str) > 255 Then
					g = 255
					Else
					g = Val(str)
					End If

					str = ReadField(4, chat, 126)
					If Val(str) > 255 Then
					b = 255
					Else
					b = Val(str)
					End If

					AddtoRichTextBox(frmComerciarUsu.CommerceConsole, Left$(chat, InStr(1, chat, "~") - 1), r, g, b, Val(ReadField(5, chat, 126)) < > 0, Val(ReadField(6, chat, 126)) < > 0)
					Else
					With FontTypes(FontIndex)
					AddtoRichTextBox(frmComerciarUsu.CommerceConsole, chat, .red, .green, .blue, .bold, .italic)

					End If
					*/
	}

	//
	// Handles the ShowMessageBox message.

	void Protocol::HandleShowMessageBox()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		//frmMensaje.msg.Caption = Buffer.ReadASCIIString()
		//frmMensaje.Show

	}

	//
	// Handles the UserIndexInServer message.

	void Protocol::HandleUserIndexInServer()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadInt16();
		//UserIndex = incomingData.ReadInteger()
	}

	//
	// Handles the UserCharIndexInServer message.

	void Protocol::HandleUserCharIndexInServer()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadInt16();
		//UserCharIndex = mIncomingBuffer->ReadInt16();
		/*UserPos = charlist(UserCharIndex).Pos

		//Are we under a roof?
		bTecho = IIf(MapData(UserPos.X, UserPos.Y).Trigger = 1 Or _
			MapData(UserPos.X, UserPos.Y).Trigger = 2 Or _
			MapData(UserPos.X, UserPos.Y).Trigger = 4, True, False)

		frmMain.Coord.Caption = UserMap & " X: " & UserPos.X & " Y: " & UserPos.Y*/
	}

	//
	// Handles the CharacterCreate message.

	void Protocol::HandleCharacterCreate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 CharIndex = mIncomingBuffer->ReadInt16();
		Int32 Body = mIncomingBuffer->ReadInt16();
		Int32 Head = mIncomingBuffer->ReadInt16();
		UInt8 Heading = mIncomingBuffer->ReadUInt8();
		UInt8 X = mIncomingBuffer->ReadUInt8();
		UInt8 Y = mIncomingBuffer->ReadUInt8();
		Int32 weapon = mIncomingBuffer->ReadInt16();
		Int32 shield = mIncomingBuffer->ReadInt16();
		Int32 helmet = mIncomingBuffer->ReadInt16();

		mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadInt16();
		//SetCharacterFx(CharIndex, Buffer.ReadInteger(), Buffer.ReadInteger())

			//.Nombre = Buffer.ReadASCIIString()
		Int32 NickColor = mIncomingBuffer->ReadUInt8();

		/*If(NickColor And eNickColor.ieCriminal) < > 0 Then
		.Criminal = 1
		Else
		.Criminal = 0
		End If

		.Atacable = (NickColor And eNickColor.ieAtacable) < > 0
		*/
		Int32 privs = mIncomingBuffer->ReadUInt8();

		/*If privs < > 0 Then
		//If the player belongs to a council AND is an admin, only whos as an admin
		If(privs And PlayerType.ChaosCouncil) < > 0 And(privs And PlayerType.User) = 0 Then
		privs = privs Xor PlayerType.ChaosCouncil
		End If

		If(privs And PlayerType.RoyalCouncil) < > 0 And(privs And PlayerType.User) = 0 Then
		privs = privs Xor PlayerType.RoyalCouncil
		End If

		//If the player is a RM, ignore other flags
		If privs And PlayerType.RoleMaster Then
		privs = PlayerType.RoleMaster
		End If

		//Log2 of the bit flags sent by the server gives our numbers ^^
		.priv = Log(privs) / Log(2)
		Else
		.priv = 0
		End If


		MakeChar(CharIndex, Body, Head, Heading, X, Y, weapon, shield, helmet)

		RefreshAllChars
			*/
	}

	void Protocol::HandleCharacterChangeNick()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 07/23/09
		//
		//***************************************************

		Int32 CharIndex = mIncomingBuffer->ReadInt16();
		//charlist(CharIndex).Nombre = incomingData.ReadASCIIString
	}

	//
	// Handles the CharacterRemove message.

	void Protocol::HandleCharacterRemove()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 CharIndex = mIncomingBuffer->ReadInt16();

		//EraseChar(CharIndex)
		//RefreshAllChars
	}

	//
	// Handles the CharacterMove message.

	void Protocol::HandleCharacterMove()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 CharIndex = mIncomingBuffer->ReadInt16();
		Int32 X = mIncomingBuffer->ReadUInt8();
		Int32 Y = mIncomingBuffer->ReadUInt8();

		/*With charlist(CharIndex)
		If.FxIndex >= 40 And.FxIndex <= 49 Then   //If it//s meditating, we remove the FX
		.FxIndex = 0
		End If

		// Play steps sounds if the user is not an admin of any kind
		If.priv < > 1 And.priv < > 2 And.priv < > 3 And.priv < > 5 And.priv < > 25 Then
		DoPasosFx(CharIndex)
		End If


		MoveCharbyPos(CharIndex, X, Y)

		RefreshAllChars*/
	}

	//
	// Handles the ForceCharMove message.

	void Protocol::HandleForceCharMove()
	{
		Int32 Direccion = mIncomingBuffer->ReadUInt8();

		/*MoveCharbyHead(UserCharIndex, Direccion)
		MoveScreen(Direccion)

		RefreshAllChars*/
	}

	//
	// Handles the CharacterChange message.

	void Protocol::HandleCharacterChange()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 21/09/2010 - C4b3z0n
		//25/08/2009: ZaMa - Changed a variable used incorrectly.
		//21/09/2010: C4b3z0n - Added code for FragShooter. If its waiting for the death of certain UserIndex, and it dies, then the capture of the screen will occur.
		//***************************************************


		Int32 CharIndex = mIncomingBuffer->ReadInt16();

		//With charlist(CharIndex)
		Int32 tempint = mIncomingBuffer->ReadInt16();

		/*If tempint < LBound(BodyData()) Or tempint > UBound(BodyData()) Then
		.Body = BodyData(0)
		.iBody = 0
		Else
		.Body = BodyData(tempint)
		.iBody = tempint
		End If*/


		Int32 headIndex = mIncomingBuffer->ReadInt16();

		/* If headIndex < LBound(HeadData()) Or headIndex > UBound(HeadData()) Then
		 .Head = HeadData(0)
		 .iHead = 0
		 Else
		 .Head = HeadData(headIndex)
		 .iHead = headIndex
		 End If

		 .muerto = (headIndex = CASPER_HEAD)*/

		mIncomingBuffer->ReadUInt8();//.Heading = incomingData.ReadByte()

		tempint = mIncomingBuffer->ReadInt16();
		//If tempint < > 0 Then.Arma = WeaponAnimData(tempint)

		tempint = mIncomingBuffer->ReadInt16();
		//If tempint < > 0 Then.Escudo = ShieldAnimData(tempint)

		tempint = mIncomingBuffer->ReadInt16();
		//If tempint < > 0 Then.Casco = CascoAnimData(tempint)

		mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadInt16();
		//SetCharacterFx(CharIndex, incomingData.ReadInteger(), incomingData.ReadInteger())

		//RefreshAllChars
	}

	//
	// Handles the ObjectCreate message.

	void Protocol::HandleObjectCreate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 X = mIncomingBuffer->ReadUInt8();
		Int32 Y = mIncomingBuffer->ReadUInt8();

		mIncomingBuffer->ReadInt16();
		//MapData(X, Y).ObjGrh.GrhIndex = incomingData.ReadInteger()

	//InitGrh(MapData(X, Y).ObjGrh, MapData(X, Y).ObjGrh.GrhIndex)
	}

	//
	// Handles the ObjectDelete message.

	void Protocol::HandleObjectDelete()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 X = mIncomingBuffer->ReadUInt8();
		Int32 Y = mIncomingBuffer->ReadUInt8();
		//MapData(X, Y).ObjGrh.GrhIndex = 0
	}

	//
	// Handles the BlockPosition message.

	void Protocol::HandleBlockPosition()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 X = mIncomingBuffer->ReadUInt8();
		Int32 Y = mIncomingBuffer->ReadUInt8();

		mIncomingBuffer->ReadUInt8();
		/*If incomingData.ReadBoolean() Then
		MapData(X, Y).Blocked = 1
		Else
		MapData(X, Y).Blocked = 0
		End If*/
	}

	//
	// Handles the PlayMIDI message.

	void Protocol::HandlePlayMIDI()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 currentMidi = mIncomingBuffer->ReadInt16();
		Int32 Loops = mIncomingBuffer->ReadInt16();

		/*If currentMidi Then
		If currentMidi > MP3_INITIAL_INDEX Then
		Audio.MusicMP3Play(App.path & "\MP3\" & currentMidi & ".mp3")
			Else
			Audio.PlayMIDI(CStr(currentMidi) & ".mid", Loops)
			End If
			End If*/

	}

	//
	// Handles the PlayWave message.

	void Protocol::HandlePlayWave()
	{
		//***************************************************
		//Autor: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 08/14/07
		//Last Modified by: Rapsodius
		//Added support for 3D Sounds.
		//***************************************************

		Int32 wave = mIncomingBuffer->ReadUInt8();
		Int32 srcX = mIncomingBuffer->ReadUInt8();
		Int32 srcY = mIncomingBuffer->ReadUInt8();

		//Audio.PlayWave(CStr(wave) & ".wav", srcX, srcY)
	}

	//
	// Handles the GuildList message.

	void Protocol::HandleGuildList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			/*
			//Clear guild//s list
			.guildslist.Clear

			GuildNames = Split(Buffer.ReadASCIIString(), SEPARATOR)

			DimInt32 i
			For i = 0 To UBound(GuildNames())
			Call.guildslist.AddItem(GuildNames(i))
			Next i

			.Show vbModeless, frmMain*/
	}

	//
	// Handles the AreaChanged message.

	void Protocol::HandleAreaChanged()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		Int32 X = mIncomingBuffer->ReadUInt8();
		Int32 Y = mIncomingBuffer->ReadUInt8();

		//CambioDeArea(X, Y)
	}

	//
	// Handles the PauseToggle message.

	void Protocol::HandlePauseToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		//pausa = Not pausa
	}

	//
	// Handles the RainToggle message.

	void Protocol::HandleRainToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//If Not InMapBounds(UserPos.X, UserPos.Y) Then Exit Sub

			/*bTecho = (MapData(UserPos.X, UserPos.Y).Trigger = 1 Or _
				MapData(UserPos.X, UserPos.Y).Trigger = 2 Or _
				MapData(UserPos.X, UserPos.Y).Trigger = 4)
			If bRain Then
			If bLluvia(UserMap) Then
			//Stop playing the rain sound
			Audio.StopWave(RainBufferIndex)
			RainBufferIndex = 0
			If bTecho Then
			Audio.PlayWave("lluviainend.wav", 0, 0, LoopStyle.Disabled)
			Else
			Audio.PlayWave("lluviaoutend.wav", 0, 0, LoopStyle.Disabled)
			End If
			frmMain.IsPlaying = PlayLoop.plNone
			End If
			End If

			bRain = Not bRain*/
	}

	//
	// Handles the CreateFX message.

	void Protocol::HandleCreateFX()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		Int32 CharIndex = mIncomingBuffer->ReadInt16();
		Int32 fX = mIncomingBuffer->ReadInt16();
		Int32 Loops = mIncomingBuffer->ReadInt16();

		//SetCharacterFx(CharIndex, fX, Loops)
	}

	//
	// Handles the UpdateUserStats message.

	void Protocol::HandleUpdateUserStats()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
		mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadInt32();
		mIncomingBuffer->ReadUInt8();
		mIncomingBuffer->ReadInt32();
		mIncomingBuffer->ReadInt32();
		/*
			UserMaxHP = incomingData.ReadInteger()
			UserMinHP = incomingData.ReadInteger()
			UserMaxMAN = incomingData.ReadInteger()
			UserMinMAN = incomingData.ReadInteger()
			UserMaxSTA = incomingData.ReadInteger()
			UserMinSTA = incomingData.ReadInteger()
			UserGLD = incomingData.ReadLong()
			UserLvl = incomingData.ReadByte()
			UserPasarNivel = incomingData.ReadLong()
			UserExp = incomingData.ReadLong()

			frmMain.lblExp.Caption = "Exp: " & UserExp & "/" & UserPasarNivel

			If UserPasarNivel > 0 Then
			frmMain.lblPorcLvl.Caption = "[" & Round(CDbl(UserExp) * CDbl(100) / CDbl(UserPasarNivel), 2) & "%]"
			Else
			frmMain.lblPorcLvl.Caption = "[N/A]"
			End If

			frmMain.GldLbl.Caption = UserGLD
			frmMain.lblLvl.Caption = UserLvl

			//Stats
			frmMain.lblMana = UserMinMAN & "/" & UserMaxMAN
			frmMain.lblVida = UserMinHP & "/" & UserMaxHP
			frmMain.lblEnergia = UserMinSTA & "/" & UserMaxSTA

			DimUInt8 bWidth

			//***************************
			If UserMaxMAN > 0 Then _
			bWidth = (((UserMinMAN / 100) / (UserMaxMAN / 100)) * 75)

			frmMain.shpMana.Width = 75 - bWidth
			frmMain.shpMana.Left = 584 + (75 - frmMain.shpMana.Width)

			frmMain.shpMana.Visible = (bWidth < > 75)
			//***************************

			bWidth = (((UserMinHP / 100) / (UserMaxHP / 100)) * 75)

			frmMain.shpVida.Width = 75 - bWidth
			frmMain.shpVida.Left = 584 + (75 - frmMain.shpVida.Width)

			frmMain.shpVida.Visible = (bWidth < > 75)
			//***************************

			bWidth = (((UserMinSTA / 100) / (UserMaxSTA / 100)) * 75)

			frmMain.shpEnergia.Width = 75 - bWidth
			frmMain.shpEnergia.Left = 584 + (75 - frmMain.shpEnergia.Width)

			frmMain.shpEnergia.Visible = (bWidth < > 75)
			//***************************

			If UserMinHP = 0 Then
			UserEstado = 1
			If frmMain.TrainingMacro Then frmMain.DesactivarMacroHechizos
			If frmMain.macrotrabajo Then frmMain.DesactivarMacroTrabajo
			Else
			UserEstado = 0
			End If

			If UserGLD >= CLng(UserLvl) * 10000 Then
			//Changes color
			frmMain.GldLbl.ForeColor = &HFF & //Red
			Else
			//Changes color
			frmMain.GldLbl.ForeColor = &HFFFF & //Yellow
			End If
			*/
	}

	//
	// Handles the WorkRequestTarget message.

	void Protocol::HandleWorkRequestTarget()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadUInt8();//UsingSkill = incomingData.ReadByte()

		// frmMain.MousePointer = 2
	 /*
		 Select Case UsingSkill
		 Case Magia
		 AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_MAGIA, 100, 100, 120, 0, 0)
		 Case Pesca
		 AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_PESCA, 100, 100, 120, 0, 0)
		 Case Robar
		 AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_ROBAR, 100, 100, 120, 0, 0)
		 Case Talar
		 AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_TALAR, 100, 100, 120, 0, 0)
		 Case Mineria
		 AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_MINERIA, 100, 100, 120, 0, 0)
		 Case FundirMetal
		 AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_FUNDIRMETAL, 100, 100, 120, 0, 0)
		 Case Proyectiles
		 AddtoRichTextBox(frmMain.RecTxt, MENSAJE_TRABAJO_PROYECTILES, 100, 100, 120, 0, 0)
		 End Select
		 */
	}

	//
	// Handles the ChangeInventorySlot message.

	void Protocol::HandleChangeInventorySlot()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
		Int32 slot = mIncomingBuffer->ReadUInt8();
		Int32 OBJIndex = mIncomingBuffer->ReadInt16();
		String Name = "";// Buffer.ReadASCIIString()
		Int32 Amount = mIncomingBuffer->ReadInt16();
		Bool Equipped = mIncomingBuffer->ReadUInt8();
		Int32 GrhIndex = mIncomingBuffer->ReadInt16();
		Int32 OBJType = mIncomingBuffer->ReadUInt8();
		Int32 MaxHit = mIncomingBuffer->ReadInt16();
		Int32 MinHit = mIncomingBuffer->ReadInt16();
		Int32 MaxDef = mIncomingBuffer->ReadInt16();
		Int32 MinDef = mIncomingBuffer->ReadInt16();
		Int32 value = mIncomingBuffer->ReadFloat();
		/*
				If Equipped Then
				Select Case OBJType
				Case eObjType.otWeapon
				frmMain.lblWeapon = MinHit & "/" & MaxHit
				UserWeaponEqpSlot = slot
				Case eObjType.otArmadura
				frmMain.lblArmor = MinDef & "/" & MaxDef
				UserArmourEqpSlot = slot
				Case eObjType.otescudo
				frmMain.lblShielder = MinDef & "/" & MaxDef
				UserHelmEqpSlot = slot
				Case eObjType.otcasco
				frmMain.lblHelm = MinDef & "/" & MaxDef
				UserShieldEqpSlot = slot
				End Select
				Else
				Select Case slot
				Case UserWeaponEqpSlot
				frmMain.lblWeapon = "0/0"
				UserWeaponEqpSlot = 0
				Case UserArmourEqpSlot
				frmMain.lblArmor = "0/0"
				UserArmourEqpSlot = 0
				Case UserHelmEqpSlot
				frmMain.lblShielder = "0/0"
				UserHelmEqpSlot = 0
				Case UserShieldEqpSlot
				frmMain.lblHelm = "0/0"
				UserShieldEqpSlot = 0
				End Select
				End If

				Inventario.SetItem(slot, OBJIndex, Amount, Equipped, GrhIndex, OBJType, MaxHit, MinHit, MaxDef, MinDef, value, Name)
				*/
	}

	// Handles the AddSlots message.
	void Protocol::HandleAddSlots()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 12/01/09
		//
		//***************************************************

		mIncomingBuffer->ReadUInt8();//MaxInventorySlots = incomingData.ReadByte
	}

	// Handles the StopWorking message.
	void Protocol::HandleStopWorking()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 12/01/09
		//
		//***************************************************
		/*
			With FontTypes(FontTypeNames.FONTTYPE_INFO)
			ShowConsoleMsg("¡Has terminado de trabajar!", .red, .green, .blue, .bold, .italic)


			If frmMain.macrotrabajo.Enabled Then frmMain.DesactivarMacroTrabajo
			*/
	}

	// Handles the CancelOfferItem message.

	void Protocol::HandleCancelOfferItem()
	{
		//***************************************************
		//Author: Torres Patricio (Pato)
		//Last Modification: 05/03/10
		//
		//***************************************************

		Int32 slot = mIncomingBuffer->ReadUInt8();

		/*With InvOfferComUsu(0)
		Amount = .Amount(slot)

		// No tiene sentido que se quiten 0 unidades
		If Amount < > 0 Then
		// Actualizo el inventario general
		frmComerciarUsu.UpdateInvCom(.OBJIndex(slot), Amount)

		// Borro el item
		Call.SetItem(slot, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "")
		End If


		// Si era el único ítem de la oferta, no puede confirmarla
		If Not frmComerciarUsu.HasAnyItem(InvOfferComUsu(0)) And _
		Not frmComerciarUsu.HasAnyItem(InvOroComUsu(1)) Then frmComerciarUsu.HabilitarConfirmar(False)

		With FontTypes(FontTypeNames.FONTTYPE_INFO)
		frmComerciarUsu.PrintCommerceMsg("¡No puedes comerciar ese objeto!", FontTypeNames.FONTTYPE_INFO)
		*/
	}

	//
	// Handles the ChangeBankSlot message.

	void Protocol::HandleChangeBankSlot()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 slot = mIncomingBuffer->ReadUInt8();
		/*
			With UserBancoInventory(slot)
			.OBJIndex = Buffer.ReadInteger()
			.Name = Buffer.ReadASCIIString()
			.Amount = Buffer.ReadInteger()
			.GrhIndex = Buffer.ReadInteger()
			.OBJType = Buffer.ReadByte()
			.MaxHit = Buffer.ReadInteger()
			.MinHit = Buffer.ReadInteger()
			.MaxDef = Buffer.ReadInteger()
			.MinDef = Buffer.ReadInteger
			.Valor = Buffer.ReadLong()

			If Comerciando Then
			InvBanco(0).SetItem(slot, .OBJIndex, .Amount, _
				.Equipped, .GrhIndex, .OBJType, .MaxHit, _
				.MinHit, .MaxDef, .MinDef, .Valor, .Name)
			End If

			*/
	}

	//
	// Handles the ChangeSpellSlot message.

	void Protocol::HandleChangeSpellSlot()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		Int32 slot = mIncomingBuffer->ReadUInt8();

		mIncomingBuffer->ReadInt16();// UserHechizos(slot) = Buffer.ReadInteger()

		/*If slot <= frmMain.hlst.ListCount Then
		 frmMain.hlst.List(slot - 1) = Buffer.ReadASCIIString()
		 Else
		 frmMain.hlst.AddItem(Buffer.ReadASCIIString())
		 End If*/

	}

	//
	// Handles the Attributes message.

	void Protocol::HandleAtributes()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
		/*
			DimInt32 i

			For i = 1 To NUMATRIBUTES
			UserAtributos(i) = incomingData.ReadByte()
			Next i

			//Show them in character creation
			If EstadoLogin = E_MODO.Dados Then
			With frmCrearPersonaje
			If.Visible Then
			For i = 1 To NUMATRIBUTES
			.lblAtributos(i).Caption = UserAtributos(i)
			Next i

			.UpdateStats
			End If

			Else
			LlegaronAtrib = True
			End If*/
	}

	//
	// Handles the BlacksmithWeapons message.

	void Protocol::HandleBlacksmithWeapons()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 Count = mIncomingBuffer->ReadInt16();

		/*ReDim ArmasHerrero(Count) As tItemsConstruibles
		ReDim HerreroMejorar(0) As tItemsConstruibles

		For i = 1 To Count
		With ArmasHerrero(i)
		.Name = Buffer.ReadASCIIString()    //Get the object//s name
		.GrhIndex = Buffer.ReadInteger()
		.LinH = Buffer.ReadInteger()        //The iron needed
		.LinP = Buffer.ReadInteger()        //The silver needed
		.LinO = Buffer.ReadInteger()        //The gold needed
		.OBJIndex = Buffer.ReadInteger()
		.Upgrade = Buffer.ReadInteger()

		Next i

		For i = 1 To MAX_LIST_ITEMS
		Set InvLingosHerreria(i) = New clsGrapchicalInventory
		Next i

		With frmHerrero
		// Inicializo los inventarios
		InvLingosHerreria(1).Initialize(DirectDraw, .picLingotes0, 3, , , , , , False)
		InvLingosHerreria(2).Initialize(DirectDraw, .picLingotes1, 3, , , , , , False)
		InvLingosHerreria(3).Initialize(DirectDraw, .picLingotes2, 3, , , , , , False)
		InvLingosHerreria(4).Initialize(DirectDraw, .picLingotes3, 3, , , , , , False)

		Call.HideExtraControls(Count)
		Call.RenderList(1, True)


		For i = 1 To Count
		With ArmasHerrero(i)
		If.Upgrade Then
		For k = 1 To Count
		If.Upgrade = ArmasHerrero(k).OBJIndex Then
		j = j + 1

		ReDim Preserve HerreroMejorar(j) As tItemsConstruibles

		HerreroMejorar(j).Name = .Name
		HerreroMejorar(j).GrhIndex = .GrhIndex
		HerreroMejorar(j).OBJIndex = .OBJIndex
		HerreroMejorar(j).UpgradeName = ArmasHerrero(k).Name
		HerreroMejorar(j).UpgradeGrhIndex = ArmasHerrero(k).GrhIndex
		HerreroMejorar(j).LinH = ArmasHerrero(k).LinH - .LinH * 0.85
		HerreroMejorar(j).LinP = ArmasHerrero(k).LinP - .LinP * 0.85
		HerreroMejorar(j).LinO = ArmasHerrero(k).LinO - .LinO * 0.85

		Exit For
		End If
		Next k
		End If

		Next i
		*/
	}

	//
	// Handles the BlacksmithArmors message.

	void Protocol::HandleBlacksmithArmors()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
		Int32 Count = mIncomingBuffer->ReadInt16();
		/*
		ReDim ArmadurasHerrero(Count) As tItemsConstruibles

		For i = 1 To Count
		With ArmadurasHerrero(i)
		.Name = Buffer.ReadASCIIString()    //Get the object//s name
		.GrhIndex = Buffer.ReadInteger()
		.LinH = Buffer.ReadInteger()        //The iron needed
		.LinP = Buffer.ReadInteger()        //The silver needed
		.LinO = Buffer.ReadInteger()        //The gold needed
		.OBJIndex = Buffer.ReadInteger()
		.Upgrade = Buffer.ReadInteger()

		Next i

		j = UBound(HerreroMejorar)

		For i = 1 To Count
		With ArmadurasHerrero(i)
		If.Upgrade Then
		For k = 1 To Count
		If.Upgrade = ArmadurasHerrero(k).OBJIndex Then
		j = j + 1

		ReDim Preserve HerreroMejorar(j) As tItemsConstruibles

		HerreroMejorar(j).Name = .Name
		HerreroMejorar(j).GrhIndex = .GrhIndex
		HerreroMejorar(j).OBJIndex = .OBJIndex
		HerreroMejorar(j).UpgradeName = ArmadurasHerrero(k).Name
		HerreroMejorar(j).UpgradeGrhIndex = ArmadurasHerrero(k).GrhIndex
		HerreroMejorar(j).LinH = ArmadurasHerrero(k).LinH - .LinH * 0.85
		HerreroMejorar(j).LinP = ArmadurasHerrero(k).LinP - .LinP * 0.85
		HerreroMejorar(j).LinO = ArmadurasHerrero(k).LinO - .LinO * 0.85

		Exit For
		End If
		Next k
		End If

		Next i
		*/
		//If we got here then packet is complete, copy data back to original queue

	}

	//
	// Handles the CarpenterObjects message.

	void Protocol::HandleCarpenterObjects()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 Count = mIncomingBuffer->ReadInt16();
		/*
		ReDim ObjCarpintero(Count) As tItemsConstruibles
		ReDim CarpinteroMejorar(0) As tItemsConstruibles

		For i = 1 To Count
		With ObjCarpintero(i)
		.Name = Buffer.ReadASCIIString()        //Get the object//s name
		.GrhIndex = Buffer.ReadInteger()
		.Madera = Buffer.ReadInteger()          //The wood needed
		.MaderaElfica = Buffer.ReadInteger()    //The elfic wood needed
		.OBJIndex = Buffer.ReadInteger()
		.Upgrade = Buffer.ReadInteger()

		Next i

		For i = 1 To MAX_LIST_ITEMS
		Set InvMaderasCarpinteria(i) = New clsGrapchicalInventory
		Next i

		With frmCarp
		// Inicializo los inventarios
		InvMaderasCarpinteria(1).Initialize(DirectDraw, .picMaderas0, 2, , , , , , False)
		InvMaderasCarpinteria(2).Initialize(DirectDraw, .picMaderas1, 2, , , , , , False)
		InvMaderasCarpinteria(3).Initialize(DirectDraw, .picMaderas2, 2, , , , , , False)
		InvMaderasCarpinteria(4).Initialize(DirectDraw, .picMaderas3, 2, , , , , , False)

		Call.HideExtraControls(Count)
		Call.RenderList(1)


		For i = 1 To Count
		With ObjCarpintero(i)
		If.Upgrade Then
		For k = 1 To Count
		If.Upgrade = ObjCarpintero(k).OBJIndex Then
		j = j + 1

		ReDim Preserve CarpinteroMejorar(j) As tItemsConstruibles

		CarpinteroMejorar(j).Name = .Name
		CarpinteroMejorar(j).GrhIndex = .GrhIndex
		CarpinteroMejorar(j).OBJIndex = .OBJIndex
		CarpinteroMejorar(j).UpgradeName = ObjCarpintero(k).Name
		CarpinteroMejorar(j).UpgradeGrhIndex = ObjCarpintero(k).GrhIndex
		CarpinteroMejorar(j).Madera = ObjCarpintero(k).Madera - .Madera * 0.85
		CarpinteroMejorar(j).MaderaElfica = ObjCarpintero(k).MaderaElfica - .MaderaElfica * 0.85

		Exit For
		End If
		Next k
		End If

		Next i
		*/
	}

	//
	// Handles the RestOK message.

	void Protocol::HandleRestOK()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		 //UserDescansar = Not UserDescansar
	}

	//
	// Handles the ErrorMessage message.

	void Protocol::HandleErrorMessage()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//MsgBox(Buffer.ReadASCIIString())
		/*
			If frmConnect.Visible And(Not frmCrearPersonaje.Visible) Then
			#If UsarWrench = 1 Then
			frmMain.Socket1.Disconnect
			frmMain.Socket1.Cleanup
			#Else
			If frmMain.Winsock1.State <> sckClosed Then _
			frmMain.Winsock1.Close
			#End If
			End If
			*/
	}

	//
	// Handles the Blind message.

	void Protocol::HandleBlind()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		//UserCiego = True
	}

	//
	// Handles the Dumb message.

	void Protocol::HandleDumb()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		 //UserEstupido = True
	}

	//
	// Handles the ShowSignal message.

	void Protocol::HandleShowSignal()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		String tmp = "";// Buffer.ReadASCIIString()
		/*
		InitCartel(tmp, Buffer.ReadInteger())
		*/
	}

	//
	// Handles the ChangeNPCInventorySlot message.

	void Protocol::HandleChangeNPCInventorySlot()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 slot = mIncomingBuffer->ReadUInt8();
		/*
		With NPCInventory(slot)
		.Name = Buffer.ReadASCIIString()
		.Amount = Buffer.ReadInteger()
		.Valor = Buffer.ReadSingle()
		.GrhIndex = Buffer.ReadInteger()
		.OBJIndex = Buffer.ReadInteger()
		.OBJType = Buffer.ReadByte()
		.MaxHit = Buffer.ReadInteger()
		.MinHit = Buffer.ReadInteger()
		.MaxDef = Buffer.ReadInteger()
		.MinDef = Buffer.ReadInteger

		*/
	}

	//
	// Handles the UpdateHungerAndThirst message.

	void Protocol::HandleUpdateHungerAndThirst()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadUInt8();// UserMaxAGU = incomingData.ReadByte()
		mIncomingBuffer->ReadUInt8();//UserMinAGU = incomingData.ReadByte()
		mIncomingBuffer->ReadUInt8();    //UserMaxHAM = incomingData.ReadByte()
		mIncomingBuffer->ReadUInt8();//UserMinHAM = incomingData.ReadByte()
		/*frmMain.lblHambre = UserMinHAM & "/" & UserMaxHAM
		frmMain.lblSed = UserMinAGU & "/" & UserMaxAGU

		DimUInt8 bWidth

		bWidth = (((UserMinHAM / 100) / (UserMaxHAM / 100)) * 75)

		frmMain.shpHambre.Width = 75 - bWidth
		frmMain.shpHambre.Left = 584 + (75 - frmMain.shpHambre.Width)

		frmMain.shpHambre.Visible = (bWidth < > 75)
		//*********************************

		bWidth = (((UserMinAGU / 100) / (UserMaxAGU / 100)) * 75)

		frmMain.shpSed.Width = 75 - bWidth
		frmMain.shpSed.Left = 584 + (75 - frmMain.shpSed.Width)

		frmMain.shpSed.Visible = (bWidth < > 75)*/

	}

	//
	// Handles the Fame message.

	void Protocol::HandleFame()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//With UserReputacion
		mIncomingBuffer->ReadInt32();//.AsesinoRep = incomingData.ReadLong()
		mIncomingBuffer->ReadInt32();    //.BandidoRep = incomingData.ReadLong()
		mIncomingBuffer->ReadInt32();//.BurguesRep = incomingData.ReadLong()
		mIncomingBuffer->ReadInt32();//.LadronesRep = incomingData.ReadLong()
		mIncomingBuffer->ReadInt32();//.NobleRep = incomingData.ReadLong()
		mIncomingBuffer->ReadInt32();//.PlebeRep = incomingData.ReadLong()
		mIncomingBuffer->ReadInt32();//.Promedio = incomingData.ReadLong()
		//

		//LlegoFama = True
	}

	//
	// Handles the MiniStats message.

	void Protocol::HandleMiniStats()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//With UserEstadisticas
		mIncomingBuffer->ReadInt32();//.CiudadanosMatados = incomingData.ReadLong()
		mIncomingBuffer->ReadInt32();//.CriminalesMatados = incomingData.ReadLong()
		mIncomingBuffer->ReadInt32();//.UsuariosMatados = incomingData.ReadLong()
		mIncomingBuffer->ReadInt16();//.NpcsMatados = incomingData.ReadInteger()
		mIncomingBuffer->ReadUInt8();//.Clase = ListaClases(incomingData.ReadByte())
		mIncomingBuffer->ReadInt32();//.PenaCarcel = incomingData.ReadLong()
		//
	}

	//
	// Handles the LevelUp message.

	void Protocol::HandleLevelUp()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		mIncomingBuffer->ReadInt16();//SkillPoints = SkillPoints + incomingData.ReadInteger()

		//frmMain.LightSkillStar(True)
	}

	//
	// Handles the AddForumMessage message.

	void Protocol::HandleAddForumMessage()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			Dim ForumType As eForumMsgType
			DimString Title
			DimString Message
			DimString Author
			Dim bAnuncio As Boolean
			Dim bSticky As Boolean

			ForumType = Buffer.ReadByte

			Title = Buffer.ReadASCIIString()
			Author = Buffer.ReadASCIIString()
			Message = Buffer.ReadASCIIString()

			If Not frmForo.ForoLimpio Then
			clsForos.ClearForums
			frmForo.ForoLimpio = True
			End If

			clsForos.AddPost(ForumAlignment(ForumType), Title, Author, Message, EsAnuncio(ForumType))

*/
	}

	//
	// Handles the ShowForumForm message.

	void Protocol::HandleShowForumForm()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
/*
			frmForo.Privilegios = incomingData.ReadByte
			frmForo.CanPostSticky = incomingData.ReadByte

			If Not MirandoForo Then
			frmForo.Show, frmMain
			End If
			*/
	}

	//
	// Handles the SetInvisible message.

	void Protocol::HandleSetInvisible()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		Int32 CharIndex = mIncomingBuffer->ReadInt16();
		mIncomingBuffer->ReadUInt8();//charlist(CharIndex).invisible = incomingData.ReadBoolean()

	}

	//
	// Handles the DiceRoll message.

	void Protocol::HandleDiceRoll()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		mIncomingBuffer->ReadUInt8();//UserAtributos(eAtributos.Fuerza) = incomingData.ReadByte()
		mIncomingBuffer->ReadUInt8();    //UserAtributos(eAtributos.Agilidad) = incomingData.ReadByte()
		mIncomingBuffer->ReadUInt8();//UserAtributos(eAtributos.Inteligencia) = incomingData.ReadByte()
		mIncomingBuffer->ReadUInt8();//UserAtributos(eAtributos.Carisma) = incomingData.ReadByte()
		mIncomingBuffer->ReadUInt8();//UserAtributos(eAtributos.Constitucion) = incomingData.ReadByte()

		//With frmCrearPersonaje
		//.lblAtributos(eAtributos.Fuerza) = UserAtributos(eAtributos.Fuerza)
		//.lblAtributos(eAtributos.Agilidad) = UserAtributos(eAtributos.Agilidad)
		//.lblAtributos(eAtributos.Inteligencia) = UserAtributos(eAtributos.Inteligencia)
		//.lblAtributos(eAtributos.Carisma) = UserAtributos(eAtributos.Carisma)
		//.lblAtributos(eAtributos.Constitucion) = UserAtributos(eAtributos.Constitucion)

		//.UpdateStats

	}

	//
	// Handles the MeditateToggle message.

	void Protocol::HandleMeditateToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		//UserMeditar = Not UserMeditar
	}

	//
	// Handles the BlindNoMore message.

	void Protocol::HandleBlindNoMore()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


	   //UserCiego = False
	}

	//
	// Handles the DumbNoMore message.

	void Protocol::HandleDumbNoMore()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


		 //UserEstupido = False
	}

	//
	// Handles the SendSkills message.

	void Protocol::HandleSendSkills()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 11/19/09
		//11/19/09: Pato - Now the server send the percentage of progress of the skills.
		//***************************************************
/*
			For i = 1 To NUMSKILLS
			UserSkills(i) = incomingData.ReadByte()
			PorcentajeSkills(i) = incomingData.ReadByte()
			Next i

			LlegaronSkills = True*/
	}

	//
	// Handles the TrainerCreatureList message.

	void Protocol::HandleTrainerCreatureList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			Dim creatures() As String
			DimInt32 i

			creatures = Split(Buffer.ReadASCIIString(), SEPARATOR)

			For i = 0 To UBound(creatures())
			frmEntrenador.lstCriaturas.AddItem(creatures(i))
			Next i
			frmEntrenador.Show, frmMain
			*/
	}

	//
	// Handles the GuildNews message.

	void Protocol::HandleGuildNews()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 11/19/09
		//11/19/09: Pato - Is optional show the frmGuildNews form
		//***************************************************
/*

			Dim guildList() As String
			DimInt32 i
			DimString sTemp

			//Get news// string
			frmGuildNews.news = Buffer.ReadASCIIString()

			//Get Enemy guilds list
			guildList = Split(Buffer.ReadASCIIString(), SEPARATOR)

			For i = 0 To UBound(guildList)
			sTemp = frmGuildNews.txtClanesGuerra.Text
			frmGuildNews.txtClanesGuerra.Text = sTemp & guildList(i) & vbCrLf
			Next i

			//Get Allied guilds list
			guildList = Split(Buffer.ReadASCIIString(), SEPARATOR)

			For i = 0 To UBound(guildList)
			sTemp = frmGuildNews.txtClanesAliados.Text
			frmGuildNews.txtClanesAliados.Text = sTemp & guildList(i) & vbCrLf
			Next i

			If ClientSetup.bGuildNews Or bShowGuildNews Then frmGuildNews.Show vbModeless, frmMain
			*/
	}

	//
	// Handles the OfferDetails message.

	void Protocol::HandleOfferDetails()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
		/*

			frmUserRequest.recievePeticion(Buffer.ReadASCIIString())
			*/
	}

	//
	// Handles the AlianceProposalsList message.

	void Protocol::HandleAlianceProposalsList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
	   /*

			Dim vsGuildList() As String
			DimInt32 i

			vsGuildList = Split(Buffer.ReadASCIIString(), SEPARATOR)

			frmPeaceProp.lista.Clear
			For i = 0 To UBound(vsGuildList())
			frmPeaceProp.lista.AddItem(vsGuildList(i))
			Next i

			frmPeaceProp.ProposalType = TIPO_PROPUESTA.ALIANZA
			frmPeaceProp.Show(vbModeless, frmMain)
			*/
	}

	//
	// Handles the PeaceProposalsList message.

	void Protocol::HandlePeaceProposalsList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
 /*
			Dim guildList() As String
			DimInt32 i

			guildList = Split(Buffer.ReadASCIIString(), SEPARATOR)

			frmPeaceProp.lista.Clear
			For i = 0 To UBound(guildList())
			frmPeaceProp.lista.AddItem(guildList(i))
			Next i

			frmPeaceProp.ProposalType = TIPO_PROPUESTA.PAZ
			frmPeaceProp.Show(vbModeless, frmMain)
*/
	}

	//
	// Handles the CharacterInfo message.

	void Protocol::HandleCharacterInfo()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			With frmCharInfo
			If.frmType = CharInfoFrmType.frmMembers Then
			.imgRechazar.Visible = False
			.imgAceptar.Visible = False
			.imgEchar.Visible = True
			.imgPeticion.Visible = False
			Else
			.imgRechazar.Visible = True
			.imgAceptar.Visible = True
			.imgEchar.Visible = False
			.imgPeticion.Visible = True
			End If

			.Nombre.Caption = Buffer.ReadASCIIString()
			.Raza.Caption = ListaRazas(Buffer.ReadByte())
			.Clase.Caption = ListaClases(Buffer.ReadByte())

			If Buffer.ReadByte() = 1 Then
			.Genero.Caption = "Hombre"
			Else
			.Genero.Caption = "Mujer"
			End If

			.Nivel.Caption = Buffer.ReadByte()
			.Oro.Caption = Buffer.ReadLong()
			.Banco.Caption = Buffer.ReadLong()

			DimInt32 reputation
			reputation = Buffer.ReadLong()

			.reputacion.Caption = reputation

			.txtPeticiones.Text = Buffer.ReadASCIIString()
			.guildactual.Caption = Buffer.ReadASCIIString()
			.txtMiembro.Text = Buffer.ReadASCIIString()

			Dim armada As Boolean
			Dim caos As Boolean

			armada = Buffer.ReadBoolean()
			caos = Buffer.ReadBoolean()

			If armada Then
			.ejercito.Caption = "Armada Real"
			ElseIf caos Then
			.ejercito.Caption = "Legión Oscura"
			End If

			.Ciudadanos.Caption = CStr(Buffer.ReadLong())
			.criminales.Caption = CStr(Buffer.ReadLong())

			If reputation > 0 Then
			.status.Caption = " Ciudadano"
			.status.ForeColor = vbBlue
			Else
			.status.Caption = " Criminal"
			.status.ForeColor = vbRed
			End If

			Call.Show(vbModeless, frmMain)

			*/
	}

	//
	// Handles the GuildLeaderInfo message.

	void Protocol::HandleGuildLeaderInfo()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			DimInt32 i
			Dim List() As String

			With frmGuildLeader
			//Get list of existing guilds
			GuildNames = Split(Buffer.ReadASCIIString(), SEPARATOR)

			//Empty the list
			Call.guildslist.Clear

			For i = 0 To UBound(GuildNames())
			Call.guildslist.AddItem(GuildNames(i))
			Next i

			//Get list of guild//s members
			GuildMembers = Split(Buffer.ReadASCIIString(), SEPARATOR)
			.Miembros.Caption = CStr(UBound(GuildMembers()) + 1)

			//Empty the list
			Call.members.Clear

			For i = 0 To UBound(GuildMembers())
			Call.members.AddItem(GuildMembers(i))
			Next i

			.txtguildnews = Buffer.ReadASCIIString()

			//Get list of join requests
			List = Split(Buffer.ReadASCIIString(), SEPARATOR)

			//Empty the list
			Call.solicitudes.Clear

			For i = 0 To UBound(List())
			Call.solicitudes.AddItem(List(i))
			Next i

			.Show, frmMain

			*/
	}

	//
	// Handles the GuildDetails message.

	void Protocol::HandleGuildDetails()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			With frmGuildBrief
			.imgDeclararGuerra.Visible = .EsLeader
			.imgOfrecerAlianza.Visible = .EsLeader
			.imgOfrecerPaz.Visible = .EsLeader

			.Nombre.Caption = Buffer.ReadASCIIString()
			.fundador.Caption = Buffer.ReadASCIIString()
			.creacion.Caption = Buffer.ReadASCIIString()
			.lider.Caption = Buffer.ReadASCIIString()
			.web.Caption = Buffer.ReadASCIIString()
			.Miembros.Caption = Buffer.ReadInteger()

			If Buffer.ReadBoolean() Then
			.eleccion.Caption = "ABIERTA"
			Else
			.eleccion.Caption = "CERRADA"
			End If

			.lblAlineacion.Caption = Buffer.ReadASCIIString()
			.Enemigos.Caption = Buffer.ReadInteger()
			.Aliados.Caption = Buffer.ReadInteger()
			.antifaccion.Caption = Buffer.ReadASCIIString()

			Dim codexStr() As String
			DimInt32 i

			codexStr = Split(Buffer.ReadASCIIString(), SEPARATOR)

			For i = 0 To 7
			.Codex(i).Caption = codexStr(i)
			Next i

			.Desc.Text = Buffer.ReadASCIIString()

			 frmGuildBrief.Show vbModeless, frmMain
			*/

	}

	//
	// Handles the ShowGuildAlign message.

	void Protocol::HandleShowGuildAlign()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 14/12/2009
		//
		//***************************************************

			//frmEligeAlineacion.Show vbModeless, frmMain
	}

	//
	// Handles the ShowGuildFundationForm message.

	void Protocol::HandleShowGuildFundationForm()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//CreandoClan = True
			//frmGuildFoundation.Show, frmMain
	}

	//
	// Handles the ParalizeOK message.

	void Protocol::HandleParalizeOK()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//UserParalizado = Not UserParalizado
	}

	//
	// Handles the ShowUserRequest message.

	void Protocol::HandleShowUserRequest()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************


			//frmUserRequest.recievePeticion(Buffer.ReadASCIIString())
			//frmUserRequest.Show(vbModeless, frmMain)

	}

	//
	// Handles the TradeOK message.

	void Protocol::HandleTradeOK()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			If frmComerciar.Visible Then
			DimInt32 i

			//Update user inventory
			For i = 1 To MAX_INVENTORY_SLOTS
			// Agrego o quito un item en su totalidad
			If Inventario.OBJIndex(i) < > InvComUsu.OBJIndex(i) Then
			With Inventario
			InvComUsu.SetItem(i, .OBJIndex(i), _
				.Amount(i), .Equipped(i), .GrhIndex(i), _
				.OBJType(i), .MaxHit(i), .MinHit(i), .MaxDef(i), .MinDef(i), _
				.Valor(i), .ItemName(i))

			// Vendio o compro cierta cantidad de un item que ya tenia
			ElseIf Inventario.Amount(i) < > InvComUsu.Amount(i) Then
			InvComUsu.ChangeSlotItemAmount(i, Inventario.Amount(i))
			End If
			Next i

			// Fill Npc inventory
			For i = 1 To 20
			// Compraron la totalidad de un item, o vendieron un item que el npc no tenia
			If NPCInventory(i).OBJIndex <> InvComNpc.OBJIndex(i) Then
			With NPCInventory(i)
			InvComNpc.SetItem(i, .OBJIndex, _
				.Amount, 0, .GrhIndex, _
				.OBJType, .MaxHit, .MinHit, .MaxDef, .MinDef, _
				.Valor, .Name)

			// Compraron o vendieron cierta cantidad (no su totalidad)
			ElseIf NPCInventory(i).Amount <> InvComNpc.Amount(i) Then
			InvComNpc.ChangeSlotItemAmount(i, NPCInventory(i).Amount)
			End If
			Next i

			End If
			*/
	}

	//
	// Handles the BankOK message.

	void Protocol::HandleBankOK()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			DimInt32 i

			If frmBancoObj.Visible Then

			For i = 1 To Inventario.MaxObjs
			With Inventario
			InvBanco(1).SetItem(i, .OBJIndex(i), .Amount(i), _
				.Equipped(i), .GrhIndex(i), .OBJType(i), .MaxHit(i), _
				.MinHit(i), .MaxDef(i), .MinDef(i), .Valor(i), .ItemName(i))

			Next i

			//Alter order according to if we bought or sold so the labels and grh remain the same
			If frmBancoObj.LasActionBuy Then
			//frmBancoObj.List1(1).ListIndex = frmBancoObj.LastIndex2
			//frmBancoObj.List1(0).ListIndex = frmBancoObj.LastIndex1
			Else
			//frmBancoObj.List1(0).ListIndex = frmBancoObj.LastIndex1
			//frmBancoObj.List1(1).ListIndex = frmBancoObj.LastIndex2
			End If

			frmBancoObj.NoPuedeMover = False
			End If
			*/
	}

	//
	// Handles the ChangeUserTradeSlot message.

	void Protocol::HandleChangeUserTradeSlot()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			OfferSlot = Buffer.ReadByte

			With Buffer
			If OfferSlot = GOLD_OFFER_SLOT Then
			InvOroComUsu(2).SetItem(1, .ReadInteger(), .ReadLong(), 0, _
				.ReadInteger(), .ReadByte(), .ReadInteger(), _
				.ReadInteger(), .ReadInteger(), .ReadInteger(), .ReadLong(), .ReadASCIIString())
			Else
			InvOfferComUsu(1).SetItem(OfferSlot, .ReadInteger(), .ReadLong(), 0, _
				.ReadInteger(), .ReadByte(), .ReadInteger(), _
				.ReadInteger(), .ReadInteger(), .ReadInteger(), .ReadLong(), .ReadASCIIString())
			End If


			frmComerciarUsu.PrintCommerceMsg(TradingUserName & " ha modificado su oferta.", FontTypeNames.FONTTYPE_VENENO)
			*/
	}

	//
	// Handles the SendNight message.

	void Protocol::HandleSendNight()
	{
		//***************************************************
		//Author: Fredy Horacio Treboux (liquid)
		//Last Modification: 01/08/07
		//
		//***************************************************

		mIncomingBuffer->ReadUInt8();
	}

	//
	// Handles the SpawnList message.

	void Protocol::HandleSpawnList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			Dim creatureList() As String
			DimInt32 i

			creatureList = Split(Buffer.ReadASCIIString(), SEPARATOR)

			For i = 0 To UBound(creatureList())
			frmSpawnList.lstCriaturas.AddItem(creatureList(i))
			Next i
			frmSpawnList.Show, frmMain
			*/
	}

	//
	// Handles the ShowSOSForm message.

	void Protocol::HandleShowSOSForm()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*
			Dim sosList() As String
			DimInt32 i

			sosList = Split(Buffer.ReadASCIIString(), SEPARATOR)

			For i = 0 To UBound(sosList())
			frmMSG.List1.AddItem(sosList(i))
			Next i

			frmMSG.Show, frmMain
			*/
	}

	//
	// Handles the ShowDenounces message.

	void Protocol::HandleShowDenounces()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 14/11/2010
		//
		//***************************************************
/*
			Dim DenounceList() As String
			DimInt32 DenounceIndex

			DenounceList = Split(Buffer.ReadASCIIString(), SEPARATOR)

			With FontTypes(FontTypeNames.FONTTYPE_GUILDMSG)
			For DenounceIndex = 0 To UBound(DenounceList())
			AddtoRichTextBox(frmMain.RecTxt, DenounceList(DenounceIndex), .red, .green, .blue, .bold, .italic)
			Next DenounceIndex

			*/
	}

	//
	// Handles the ShowSOSForm message.

	void Protocol::HandleShowPartyForm()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 11/26/09
		//
		//***************************************************
/*
			Dim members() As String
			DimInt32 i

			EsPartyLeader = CBool(Buffer.ReadByte())

			members = Split(Buffer.ReadASCIIString(), SEPARATOR)
			For i = 0 To UBound(members())
			frmParty.lstMembers.AddItem(members(i))
			Next i

			frmParty.lblTotalExp.Caption = Buffer.ReadLong
			frmParty.Show, frmMain
			*/
	}



	//
	// Handles the ShowMOTDEditionForm message.

	void Protocol::HandleShowMOTDEditionForm()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//*************************************Su**************
/*            frmCambiaMotd.txtMotd.Text = Buffer.ReadASCIIString()
			frmCambiaMotd.Show, frmMain
			*/
	}

	//
	// Handles the ShowGMPanelForm message.

	void Protocol::HandleShowGMPanelForm()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

		   // frmPanelGm.Show vbModeless, frmMain
	}

	//
	// Handles the UserNameList message.

	void Protocol::HandleUserNameList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*            Dim userList() As String
			DimInt32 i

			userList = Split(Buffer.ReadASCIIString(), SEPARATOR)

			If frmPanelGm.Visible Then
			frmPanelGm.cboListaUsus.Clear
			For i = 0 To UBound(userList())
			frmPanelGm.cboListaUsus.AddItem(userList(i))
			Next i
			If frmPanelGm.cboListaUsus.ListCount > 0 Then frmPanelGm.cboListaUsus.ListIndex = 0
			End If
			*/
	}

	//
	// Handles the Pong message.

	void Protocol::HandlePong()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************

			//AddtoRichTextBox(frmMain.RecTxt, "El ping es " & (GetTickCount - pingTime) & " ms.", 255, 0, 0, True, False, True)

			//pingTime = 0
	}

	//
	// Handles the Pong message.

	void Protocol::HandleGuildMemberInfo()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 05/17/06
		//
		//***************************************************
/*

			With frmGuildMember
			//Clear guild//s list
			.lstClanes.Clear

			GuildNames = Split(Buffer.ReadASCIIString(), SEPARATOR)

			DimInt32 i
			For i = 0 To UBound(GuildNames())
			Call.lstClanes.AddItem(GuildNames(i))
			Next i

			//Get list of guild//s members
			GuildMembers = Split(Buffer.ReadASCIIString(), SEPARATOR)
			.lblCantMiembros.Caption = CStr(UBound(GuildMembers()) + 1)

			//Empty the list
			Call.lstMiembros.Clear

			For i = 0 To UBound(GuildMembers())
			Call.lstMiembros.AddItem(GuildMembers(i))
			Next i
			.Show vbModeless, frmMain*/
	}

	//
	// Handles the UpdateTag message.

	void Protocol::HandleUpdateTagAndStatus()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//
		//***************************************************
/*

			DimInt32 CharIndex
			DimUInt8 NickColor
			DimString UserTag

			CharIndex = Buffer.ReadInteger()
			NickColor = Buffer.ReadByte()
			UserTag = Buffer.ReadASCIIString()

			//Update char status adn tag!
			With charlist(CharIndex)
			If(NickColor And eNickColor.ieCriminal) < > 0 Then
			.Criminal = 1
			Else
			.Criminal = 0
			End If

			.Atacable = (NickColor And eNickColor.ieAtacable) < > 0

			.Nombre = UserTag

			*/
	}


	//
	// Writes the "LoginExistingChar" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteLoginExistingChar(const char* userName, const char* userPass)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "LoginExistingChar" message to the outgoing data buffer
		//***************************************************


		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::LoginExistingChar));

		mOutgoingBuffer->WriteString(userName);

		mOutgoingBuffer->WriteString(userPass);

		mOutgoingBuffer->WriteUInt8(1);
		mOutgoingBuffer->WriteUInt8(0);
		mOutgoingBuffer->WriteUInt8(0);

	}

	//
	// Writes the "ThrowDices" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteThrowDices()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ThrowDices" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ThrowDices));
	}

	//
	// Writes the "LoginNewChar" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteLoginNewChar(const char* userName, const char* userPass)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "LoginNewChar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::LoginNewChar));

		mOutgoingBuffer->WriteString(userName);

		mOutgoingBuffer->WriteString(userPass);


		mOutgoingBuffer->WriteUInt8(1);
		mOutgoingBuffer->WriteUInt8(0);
		mOutgoingBuffer->WriteUInt8(0);


		//mOutgoingBuffer->WriteUInt8(UserRaza);
		//mOutgoingBuffer->WriteUInt8(UserSexo);
		//mOutgoingBuffer->WriteUInt8(UserClase);
		//mOutgoingBuffer->WriteInt16(UserHead);

		   // //Call.WriteString(UserEmail)

		//mOutgoingBuffer->WriteUInt8(UserHogar);

	}

	//
	// Writes the "Talk" message to the outgoing data buffer.
	//
	// @param    chat The chat text to be sent.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTalk(String chat)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Talk" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Talk));

		////Call.WriteString(chat)

	}

	//
	// Writes the "Yell" message to the outgoing data buffer.
	//
	// @param    chat The chat text to be sent.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteYell(String chat)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Yell" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Yell));

		// //Call.WriteString(chat)

	}

	//
	// Writes the "Whisper" message to the outgoing data buffer.
	//
	// @param    charIndex The index of the char to whom to whisper.
	// @param    chat The chat text to be sent to the user.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWhisper(String CharName, String chat)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 03/12/10
		//Writes the "Whisper" message to the outgoing data buffer
		//03/12/10: Enanoh - Ahora se envía el nick y no el charindex.
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Whisper));

		////Call.WriteString(CharName)

		////Call.WriteString(chat)

	}

	//
	// Writes the "Walk" message to the outgoing data buffer.
	//
	// @param    heading The direction in wich the user is moving.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWalk(UInt8 Heading)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Walk" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Walk));

		mOutgoingBuffer->WriteUInt8(Heading);

	}

	//
	// Writes the "RequestPositionUpdate" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestPositionUpdate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestPositionUpdate" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestPositionUpdate));
	}

	//
	// Writes the "Attack" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteAttack()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Attack" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Attack));
	}

	//
	// Writes the "PickUp" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePickUp()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PickUp" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PickUp));
	}

	//
	// Writes the "SafeToggle" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSafeToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SafeToggle" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::SafeToggle));
	}

	//
	// Writes the "ResuscitationSafeToggle" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteResuscitationToggle()
	{
		//**************************************************************
		//Author: Rapsodius
		//Creation Date: 10/10/07
		//Writes the Resuscitation safe toggle packet to the outgoing data buffer.
		//**************************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ResuscitationSafeToggle));
	}

	//
	// Writes the "RequestGuildLeaderInfo" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestGuildLeaderInfo()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestGuildLeaderInfo" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestGuildLeaderInfo));
	}

	void Protocol::WriteRequestPartyForm()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 11/26/09
		//Writes the "RequestPartyForm" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestPartyForm));

	}

	//
	// Writes the "ItemUpgrade" message to the outgoing data buffer.
	//
	// @param    ItemIndex The index to the item to upgrade.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteItemUpgrade(Int32 ItemIndex)
	{
		//***************************************************
		//Author: Torres Patricio (Pato)
		//Last Modification: 12/09/09
		//Writes the "ItemUpgrade" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ItemUpgrade));
		mOutgoingBuffer->WriteInt16(ItemIndex);
	}

	//
	// Writes the "RequestAtributes" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestAtributes()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestAtributes" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestAtributes));
	}

	//
	// Writes the "RequestFame" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestFame()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestFame" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestFame));
	}

	//
	// Writes the "RequestSkills" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestSkills()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestSkills" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestSkills));
	}

	//
	// Writes the "RequestMiniStats" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestMiniStats()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestMiniStats" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestMiniStats));
	}

	//
	// Writes the "CommerceEnd" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCommerceEnd()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CommerceEnd" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CommerceEnd));
	}

	//
	// Writes the "UserCommerceEnd" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUserCommerceEnd()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "UserCommerceEnd" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::UserCommerceEnd));
	}

	//
	// Writes the "UserCommerceConfirm" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUserCommerceConfirm()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 14/12/2009
		//Writes the "UserCommerceConfirm" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::UserCommerceConfirm));
	}

	//
	// Writes the "BankEnd" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBankEnd()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BankEnd" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::BankEnd));
	}

	//
	// Writes the "UserCommerceOk" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUserCommerceOk()
	{
		//***************************************************
		//Author: Fredy Horacio Treboux (liquid)
		//Last Modification: 01/10/07
		//Writes the "UserCommerceOk" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::UserCommerceOk));
	}

	//
	// Writes the "UserCommerceReject" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUserCommerceReject()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "UserCommerceReject" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::UserCommerceReject));
	}

	//
	// Writes the "Drop" message to the outgoing data buffer.
	//
	// @param    slot Inventory slot where the item to drop is.
	// @param    amount Number of items to drop.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteDrop(UInt8 slot, Int32 Amount)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Drop" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Drop));

		mOutgoingBuffer->WriteUInt8(slot);
		mOutgoingBuffer->WriteInt16(Amount);

	}

	//
	// Writes the "CastSpell" message to the outgoing data buffer.
	//
	// @param    slot Spell List slot where the spell to cast is.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCastSpell(UInt8 slot)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CastSpell" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CastSpell));

		mOutgoingBuffer->WriteUInt8(slot);

	}

	//
	// Writes the "LeftClick" message to the outgoing data buffer.
	//
	// @param    x Tile coord in the x-axis in which the user clicked.
	// @param    y Tile coord in the y-axis in which the user clicked.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteLeftClick(UInt8 X, UInt8 Y)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "LeftClick" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::LeftClick));

		mOutgoingBuffer->WriteUInt8(X);
		mOutgoingBuffer->WriteUInt8(Y);

	}

	//
	// Writes the "DoubleClick" message to the outgoing data buffer.
	//
	// @param    x Tile coord in the x-axis in which the user clicked.
	// @param    y Tile coord in the y-axis in which the user clicked.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteDoubleClick(UInt8 X, UInt8 Y)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "DoubleClick" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::DoubleClick));

		mOutgoingBuffer->WriteUInt8(X);
		mOutgoingBuffer->WriteUInt8(Y);

	}

	//
	// Writes the "Work" message to the outgoing data buffer.
	//
	// @param    skill The skill which the user attempts to use.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWork(UInt8 Skill)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Work" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Work));

		mOutgoingBuffer->WriteUInt8(Skill);

	}

	//
	// Writes the "UseSpellMacro" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUseSpellMacro()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "UseSpellMacro" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::UseSpellMacro));
	}

	//
	// Writes the "UseItem" message to the outgoing data buffer.
	//
	// @param    slot Invetory slot where the item to use is.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUseItem(UInt8 slot)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "UseItem" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::UseItem));

		mOutgoingBuffer->WriteUInt8(slot);

	}

	//
	// Writes the "CraftBlacksmith" message to the outgoing data buffer.
	//
	// @param    item Index of the item to craft in the list sent by the server.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCraftBlacksmith(Int32 Item)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CraftBlacksmith" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CraftBlacksmith));

		mOutgoingBuffer->WriteInt16(Item);

	}

	//
	// Writes the "CraftCarpenter" message to the outgoing data buffer.
	//
	// @param    item Index of the item to craft in the list sent by the server.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCraftCarpenter(Int32 Item)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CraftCarpenter" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CraftCarpenter));

		mOutgoingBuffer->WriteInt16(Item);

	}

	//
	// Writes the "ShowGuildNews" message to the outgoing data buffer.
	//

	void Protocol::WriteShowGuildNews()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 21/02/2010
		//Writes the "ShowGuildNews" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ShowGuildNews));
	}


	//
	// Writes the "WorkLeftClick" message to the outgoing data buffer.
	//
	// @param    x Tile coord in the x-axis in which the user clicked.
	// @param    y Tile coord in the y-axis in which the user clicked.
	// @param    skill The skill which the user attempts to use.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWorkLeftClick(UInt8 X, UInt8  Y, UInt8 Skill)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "WorkLeftClick" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::WorkLeftClick));

		mOutgoingBuffer->WriteUInt8(X);
		mOutgoingBuffer->WriteUInt8(Y);

		mOutgoingBuffer->WriteUInt8(Skill);

	}

	//
	// Writes the "CreateNewGuild" message to the outgoing data buffer.
	//
	// @param    desc    The guild//s description
	// @param    name    The guild//s name
	// @param    site    The guild//s website
	// @param    codex   Array of all rules of the guild.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCreateNewGuild(String Desc, String Name, String Site)//, ByRef Codex() As String)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CreateNewGuild" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CreateNewGuild));

		////Call.WriteString(Desc)
		////Call.WriteString(Name)
		////Call.WriteString(Site)

		/*For i = LBound(Codex()) To UBound(Codex())
		temp = temp & Codex(i) & SEPARATOR
		Next i

		If Len(temp) Then _
		temp = Left$(temp, Len(temp) - 1)

		//Call.WriteString(temp)*/

	}

	//
	// Writes the "SpellInfo" message to the outgoing data buffer.
	//
	// @param    slot Spell List slot where the spell which//s info is requested is.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSpellInfo(UInt8 slot)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SpellInfo" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::SpellInfo));

		mOutgoingBuffer->WriteUInt8(slot);

	}

	//
	// Writes the "EquipItem" message to the outgoing data buffer.
	//
	// @param    slot Invetory slot where the item to equip is.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteEquipItem(UInt8 slot)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "EquipItem" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::EquipItem));

		mOutgoingBuffer->WriteUInt8(slot);

	}

	//
	// Writes the "ChangeHeading" message to the outgoing data buffer.
	//
	// @param    heading The direction in wich the user is moving.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeHeading(UInt8 Heading)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChangeHeading" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ChangeHeading));

		mOutgoingBuffer->WriteUInt8(Heading);

	}

	//
	// Writes the "ModifySkills" message to the outgoing data buffer.
	//
	// @param    skillEdt a-based array containing for each skill the number of points to add to it.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteModifySkills()//ByRef skillEdt() As Byte)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ModifySkills" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ModifySkills));

		/*For i = 1 To NUMSKILLS
		mOutgoingBuffer->WriteUInt8(skillEdt(i))
		Next i*/

	}

	//
	// Writes the "Train" message to the outgoing data buffer.
	//
	// @param    creature Position within the list provided by the server of the creature to train against.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTrain(UInt8 creature)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Train" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Train));

		mOutgoingBuffer->WriteUInt8(creature);

	}

	//
	// Writes the "CommerceBuy" message to the outgoing data buffer.
	//
	// @param    slot Position within the NPC//s inventory in which the desired item is.
	// @param    amount Number of items to buy.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCommerceBuy(UInt8 slot, Int32 Amount)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CommerceBuy" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CommerceBuy));

		mOutgoingBuffer->WriteUInt8(slot);
		mOutgoingBuffer->WriteInt16(Amount);

	}

	//
	// Writes the "BankExtractItem" message to the outgoing data buffer.
	//
	// @param    slot Position within the bank in which the desired item is.
	// @param    amount Number of items to extract.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBankExtractItem(UInt8 slot, Int32 Amount)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BankExtractItem" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::BankExtractItem));

		mOutgoingBuffer->WriteUInt8(slot);
		mOutgoingBuffer->WriteInt16(Amount);

	}

	//
	// Writes the "CommerceSell" message to the outgoing data buffer.
	//
	// @param    slot Position within user inventory in which the desired item is.
	// @param    amount Number of items to sell.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCommerceSell(UInt8 slot, Int32 Amount)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CommerceSell" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CommerceSell));

		mOutgoingBuffer->WriteUInt8(slot);
		mOutgoingBuffer->WriteInt16(Amount);

	}

	//
	// Writes the "BankDeposit" message to the outgoing data buffer.
	//
	// @param    slot Position within the user inventory in which the desired item is.
	// @param    amount Number of items to deposit.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBankDeposit(UInt8 slot, Int32 Amount)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BankDeposit" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::BankDeposit));

		mOutgoingBuffer->WriteUInt8(slot);
		mOutgoingBuffer->WriteInt16(Amount);

	}

	//
	// Writes the "ForumPost" message to the outgoing data buffer.
	//
	// @param    title The message//s title.
	// @param    message The body of the message.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteForumPost(String Title, String Message, UInt8 ForumMsgType)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ForumPost" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ForumPost));

		mOutgoingBuffer->WriteUInt8(ForumMsgType);
		////Call.WriteString(Title)
		////Call.WriteString(Message)

	}

	//
	// Writes the "MoveSpell" message to the outgoing data buffer.
	//
	// @param    upwards True if the spell will be moved up in the list, False if it will be moved downwards.
	// @param    slot Spell List slot where the spell which//s info is requested is.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteMoveSpell(Bool upwards, UInt8 slot)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "MoveSpell" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::MoveSpell));

		mOutgoingBuffer->WriteUInt8(upwards);
		mOutgoingBuffer->WriteUInt8(slot);

	}

	//
	// Writes the "MoveBank" message to the outgoing data buffer.
	//
	// @param    upwards True if the item will be moved up in the list, False if it will be moved downwards.
	// @param    slot Bank List slot where the item which//s info is requested is.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteMoveBank(Bool upwards, UInt8 slot)
	{
		//***************************************************
		//Author: Torres Patricio (Pato)
		//Last Modification: 06/14/09
		//Writes the "MoveBank" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::MoveBank));

		mOutgoingBuffer->WriteUInt8(upwards);
		mOutgoingBuffer->WriteUInt8(slot);

	}

	//
	// Writes the "ClanCodexUpdate" message to the outgoing data buffer.
	//
	// @param    desc New description of the clan.
	// @param    codex New codex of the clan.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteClanCodexUpdate(String Desc)//, ByRef Codex() As String)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ClanCodexUpdate" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ClanCodexUpdate));

		/*///Call.WriteString(Desc)

		For i = LBound(Codex()) To UBound(Codex())
		temp = temp & Codex(i) & SEPARATOR
		Next i

		If Len(temp) Then _
		temp = Left$(temp, Len(temp) - 1)

		//Call.WriteString(temp)*/

	}

	//
	// Writes the "UserCommerceOffer" message to the outgoing data buffer.
	//
	// @param    slot Position within user inventory in which the desired item is.
	// @param    amount Number of items to offer.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUserCommerceOffer(UInt8 slot, Int32 Amount, UInt8 OfferSlot)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "UserCommerceOffer" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::UserCommerceOffer));

		mOutgoingBuffer->WriteUInt8(slot);
		mOutgoingBuffer->WriteInt32(Amount);
		mOutgoingBuffer->WriteUInt8(OfferSlot);

	}

	void Protocol::WriteCommerceChat(String chat)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 03/12/2009
		//Writes the "CommerceChat" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CommerceChat));

		////Call.WriteString(chat)

	}


	//
	// Writes the "GuildAcceptPeace" message to the outgoing data buffer.
	//
	// @param    guild The guild whose peace offer is accepted.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildAcceptPeace(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildAcceptPeace" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildAcceptPeace));

		// //Call.WriteString(guild)

	}

	//
	// Writes the "GuildRejectAlliance" message to the outgoing data buffer.
	//
	// @param    guild The guild whose aliance offer is rejected.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildRejectAlliance(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildRejectAlliance" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildRejectAlliance));

		//   //Call.WriteString(guild)

	}

	//
	// Writes the "GuildRejectPeace" message to the outgoing data buffer.
	//
	// @param    guild The guild whose peace offer is rejected.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildRejectPeace(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildRejectPeace" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildRejectPeace));

		////Call.WriteString(guild)

	}

	//
	// Writes the "GuildAcceptAlliance" message to the outgoing data buffer.
	//
	// @param    guild The guild whose aliance offer is accepted.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildAcceptAlliance(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildAcceptAlliance" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildAcceptAlliance));

		//Call.WriteString(guild)

	}

	//
	// Writes the "GuildOfferPeace" message to the outgoing data buffer.
	//
	// @param    guild The guild to whom peace is offered.
	// @param    proposal The text to s the proposal.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildOfferPeace(String guild, String proposal)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildOfferPeace" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildOfferPeace));

		//Call.WriteString(guild)
		//Call.WriteString(proposal)

	}

	//
	// Writes the "GuildOfferAlliance" message to the outgoing data buffer.
	//
	// @param    guild The guild to whom an aliance is offered.
	// @param    proposal The text to s the proposal.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildOfferAlliance(String guild, String proposal)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildOfferAlliance" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildOfferAlliance));

		//Call.WriteString(guild)
		//Call.WriteString(proposal)

	}

	//
	// Writes the "GuildAllianceDetails" message to the outgoing data buffer.
	//
	// @param    guild The guild whose aliance proposal//s details are requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildAllianceDetails(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildAllianceDetails" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildAllianceDetails));

		//Call.WriteString(guild)

	}

	//
	// Writes the "GuildPeaceDetails" message to the outgoing data buffer.
	//
	// @param    guild The guild whose peace proposal//s details are requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildPeaceDetails(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildPeaceDetails" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildPeaceDetails));

		//Call.WriteString(guild)

	}

	//
	// Writes the "GuildRequestJoinerInfo" message to the outgoing data buffer.
	//
	// @param    username The user who wants to join the guild whose info is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildRequestJoinerInfo(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildRequestJoinerInfo" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildRequestJoinerInfo));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "GuildAlliancePropList" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildAlliancePropList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildAlliancePropList" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildAlliancePropList));
	}

	//
	// Writes the "GuildPeacePropList" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildPeacePropList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildPeacePropList" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildPeacePropList));
	}

	//
	// Writes the "GuildDeclareWar" message to the outgoing data buffer.
	//
	// @param    guild The guild to which to declare war.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildDeclareWar(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildDeclareWar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildDeclareWar));

		//Call.WriteString(guild)

	}

	//
	// Writes the "GuildNewWebsite" message to the outgoing data buffer.
	//
	// @param    url The guild//s new website//s URL.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildNewWebsite(String URL)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildNewWebsite" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildNewWebsite));

		//Call.WriteString(URL)

	}

	//
	// Writes the "GuildAcceptNewMember" message to the outgoing data buffer.
	//
	// @param    username The name of the accepted player.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildAcceptNewMember(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildAcceptNewMember" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildAcceptNewMember));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "GuildRejectNewMember" message to the outgoing data buffer.
	//
	// @param    username The name of the rejected player.
	// @param    reason The reason for which the player was rejected.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildRejectNewMember(String UserName, String Reason)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildRejectNewMember" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildRejectNewMember));

		//Call.WriteString(UserName)
		//Call.WriteString(Reason)

	}

	//
	// Writes the "GuildKickMember" message to the outgoing data buffer.
	//
	// @param    username The name of the kicked player.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildKickMember(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildKickMember" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildKickMember));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "GuildUpdateNews" message to the outgoing data buffer.
	//
	// @param    news The news to be posted.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildUpdateNews(String news)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildUpdateNews" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildUpdateNews));

		//Call.WriteString(news)

	}

	//
	// Writes the "GuildMemberInfo" message to the outgoing data buffer.
	//
	// @param    username The user whose info is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildMemberInfo(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildMemberInfo" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildMemberInfo));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "GuildOpenElections" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildOpenElections()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildOpenElections" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildOpenElections));
	}

	//
	// Writes the "GuildRequestMembership" message to the outgoing data buffer.
	//
	// @param    guild The guild to which to request membership.
	// @param    application The user//s application sheet.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildRequestMembership(String guild, String Application)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildRequestMembership" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildRequestMembership));

		//Call.WriteString(guild)
		//Call.WriteString(Application)

	}

	//
	// Writes the "GuildRequestDetails" message to the outgoing data buffer.
	//
	// @param    guild The guild whose details are requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildRequestDetails(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildRequestDetails" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildRequestDetails));

		//Call.WriteString(guild)

	}

	//
	// Writes the "Online" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteOnline()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Online" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Online));
	}

	//
	// Writes the "Quit" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteQuit()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 08/16/08
		//Writes the "Quit" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Quit));
	}

	//
	// Writes the "GuildLeave" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildLeave()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildLeave" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildLeave));
	}

	//
	// Writes the "RequestAccountState" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestAccountState()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestAccountState" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestAccountState));
	}

	//
	// Writes the "PetStand" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePetStand()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PetStand" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PetStand));
	}

	//
	// Writes the "PetFollow" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePetFollow()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PetFollow" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PetFollow));
	}

	//
	// Writes the "ReleasePet" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteReleasePet()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 18/11/2009
		//Writes the "ReleasePet" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ReleasePet));
	}


	//
	// Writes the "TrainList" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTrainList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "TrainList" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::TrainList));
	}

	//
	// Writes the "Rest" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRest()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Rest" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Rest));
	}

	//
	// Writes the "Meditate" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteMeditate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Meditate" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Meditate));
	}

	//
	// Writes the "Resucitate" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteResucitate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Resucitate" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Resucitate));
	}

	//
	// Writes the "Consultation" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteConsultation()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 01/05/2010
		//Writes the "Consultation" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Consultation));

	}

	//
	// Writes the "Heal" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteHeal()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Heal" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Heal));
	}

	//
	// Writes the "Help" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteHelp()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Help" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Help));
	}

	//
	// Writes the "RequestStats" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestStats()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestStats" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestStats));
	}

	//
	// Writes the "CommerceStart" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCommerceStart()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CommerceStart" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CommerceStart));
	}

	//
	// Writes the "BankStart" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBankStart()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BankStart" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::BankStart));
	}

	//
	// Writes the "Enlist" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteEnlist()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Enlist" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Enlist));
	}

	//
	// Writes the "Information" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteInformation()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Information" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Information));
	}

	//
	// Writes the "Reward" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteReward()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Reward" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Reward));
	}

	//
	// Writes the "RequestMOTD" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestMOTD()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestMOTD" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RequestMOTD));
	}

	//
	// Writes the "UpTime" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUpTime()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "UpTime" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Uptime));
	}

	//
	// Writes the "PartyLeave" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePartyLeave()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PartyLeave" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PartyLeave));
	}

	//
	// Writes the "PartyCreate" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePartyCreate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PartyCreate" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PartyCreate));
	}

	//
	// Writes the "PartyJoin" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePartyJoin()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PartyJoin" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PartyJoin));
	}

	//
	// Writes the "Inquiry" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteInquiry()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Inquiry" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Inquiry));
	}

	//
	// Writes the "GuildMessage" message to the outgoing data buffer.
	//
	// @param    message The message to send to the guild.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildRequestDetails" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildMessage));

		//Call.WriteString(Message)

	}

	//
	// Writes the "PartyMessage" message to the outgoing data buffer.
	//
	// @param    message The message to send to the party.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePartyMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PartyMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PartyMessage));

		//Call.WriteString(Message)

	}

	//
	// Writes the "CentinelReport" message to the outgoing data buffer.
	//
	// @param    number The number to report to the centinel.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCentinelReport(Int32 number)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CentinelReport" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CentinelReport));

		mOutgoingBuffer->WriteInt16(number);

	}

	//
	// Writes the "GuildOnline" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildOnline()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildOnline" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildOnline));
	}

	//
	// Writes the "PartyOnline" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePartyOnline()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PartyOnline" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PartyOnline));
	}

	//
	// Writes the "CouncilMessage" message to the outgoing data buffer.
	//
	// @param    message The message to send to the other council members.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCouncilMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CouncilMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::CouncilMessage));

		//Call.WriteString(Message)

	}

	//
	// Writes the "RoleMasterRequest" message to the outgoing data buffer.
	//
	// @param    message The message to send to the role masters.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRoleMasterRequest(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RoleMasterRequest" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::RoleMasterRequest));

		//Call.WriteString(Message)

	}

	//
	// Writes the "GMRequest" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGMRequest()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GMRequest" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMRequest));
	}

	//
	// Writes the "BugReport" message to the outgoing data buffer.
	//
	// @param    message The message explaining the reported bug.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBugReport(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BugReport" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::bugReport));

		//Call.WriteString(Message)

	}

	//
	// Writes the "ChangeDescription" message to the outgoing data buffer.
	//
	// @param    desc The new description of the user//s character.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeDescription(String Desc)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChangeDescription" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ChangeDescription));

		//Call.WriteString(Desc)

	}

	//
	// Writes the "GuildVote" message to the outgoing data buffer.
	//
	// @param    username The user to vote for clan leader.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildVote(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildVote" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildVote));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "Punishments" message to the outgoing data buffer.
	//
	// @param    username The user whose//s  punishments are requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePunishments(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Punishments" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Punishments));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "ChangePassword" message to the outgoing data buffer.
	//
	// @param    oldPass Previous password.
	// @param    newPass New password.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangePassword(String oldPass, String newPass)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 10/10/07
		//Last Modified By: Rapsodius
		//Writes the "ChangePassword" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ChangePassword));


		//Call.WriteString(oldPass)
		//Call.WriteString(newPass)

	}

	//
	// Writes the "Gamble" message to the outgoing data buffer.
	//
	// @param    amount The amount to gamble.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGamble(Int32 Amount)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Gamble" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Gamble));

		mOutgoingBuffer->WriteInt16(Amount);

	}

	//
	// Writes the "InquiryVote" message to the outgoing data buffer.
	//
	// @param    opt The chosen option to vote for.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteInquiryVote(UInt8 opt)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "InquiryVote" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::InquiryVote));

		mOutgoingBuffer->WriteUInt8(opt);

	}

	//
	// Writes the "LeaveFaction" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteLeaveFaction()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "LeaveFaction" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::LeaveFaction));
	}

	//
	// Writes the "BankExtractGold" message to the outgoing data buffer.
	//
	// @param    amount The amount of money to extract from the bank.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBankExtractGold(Int32 Amount)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BankExtractGold" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::BankExtractGold));

		mOutgoingBuffer->WriteInt32(Amount);

	}

	//
	// Writes the "BankDepositGold" message to the outgoing data buffer.
	//
	// @param    amount The amount of money to deposit in the bank.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBankDepositGold(Int32 Amount)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BankDepositGold" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::BankDepositGold));

		mOutgoingBuffer->WriteInt32(Amount);

	}

	//
	// Writes the "Denounce" message to the outgoing data buffer.
	//
	// @param    message The message to s the denounce.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteDenounce(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Denounce" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Denounce));

		//Call.WriteString(Message)

	}

	//
	// Writes the "GuildFundate" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildFundate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 03/21/2001
		//Writes the "GuildFundate" message to the outgoing data buffer
		//14/12/2009: ZaMa - Now first checks if the user can foundate a guild.
		//03/21/2001: Pato - Deleted de clanType param.
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildFundate));
	}

	//
	// Writes the "GuildFundation" message to the outgoing data buffer.
	//
	// @param    clanType The alignment of the clan to be founded.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildFundation(UInt8 clanType)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 14/12/2009
		//Writes the "GuildFundation" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GuildFundation));

		mOutgoingBuffer->WriteUInt8(clanType);

	}

	//
	// Writes the "PartyKick" message to the outgoing data buffer.
	//
	// @param    username The user to kick fro mthe party.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePartyKick(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PartyKick" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PartyKick));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "PartySetLeader" message to the outgoing data buffer.
	//
	// @param    username The user to set as the party//s leader.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePartySetLeader(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PartySetLeader" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PartySetLeader));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "PartyAcceptMember" message to the outgoing data buffer.
	//
	// @param    username The user to accept into the party.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePartyAcceptMember(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "PartyAcceptMember" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::PartyAcceptMember));

		//Call.WriteString(UserName)

	}

	//
	// Writes the "GuildMemberList" message to the outgoing data buffer.
	//
	// @param    guild The guild whose member list is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildMemberList(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildMemberList" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.GuildMemberList)

		//Call.WriteString(guild)

	}

	//
	// Writes the "InitCrafting" message to the outgoing data buffer.
	//
	// @param    Cantidad The final aumont of item to craft.
	// @param    NroPorCiclo The amount of items to craft per cicle.

	void Protocol::WriteInitCrafting(Int32 cantidad, Int32 NroPorCiclo)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 29/01/2010
		//Writes the "InitCrafting" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::InitCrafting));
		mOutgoingBuffer->WriteInt32(cantidad);

		mOutgoingBuffer->WriteInt16(NroPorCiclo);

	}

	//
	// Writes the "Home" message to the outgoing data buffer.
	//
	void Protocol::WriteHome()
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 01/06/10
		//Writes the "Home" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Home));

	}



	//
	// Writes the "GMMessage" message to the outgoing data buffer.
	//
	// @param    message The message to be sent to the other GMs online.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGMMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GMMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.GMMessage)
		//Call.WriteString(Message)

	}

	//
	// Writes the "ShowName" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteShowName()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ShowName" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.showName)
	}

	//
	// Writes the "OnlineRoyalArmy" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteOnlineRoyalArmy()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "OnlineRoyalArmy" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.OnlineRoyalArmy)
	}

	//
	// Writes the "OnlineChaosLegion" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteOnlineChaosLegion()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "OnlineChaosLegion" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.OnlineChaosLegion)
	}

	//
	// Writes the "GoNearby" message to the outgoing data buffer.
	//
	// @param    username The suer to approach.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGoNearby(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GoNearby" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.GoNearby)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "Comment" message to the outgoing data buffer.
	//
	// @param    message The message to leave in the log as a comment.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteComment(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Comment" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.Comment)

		//Call.WriteString(Message)

	}

	//
	// Writes the "ServerTime" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteServerTime()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ServerTime" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//    outgoingData.WriteByte(eGMCommands.serverTime)
	}

	//
	// Writes the "Where" message to the outgoing data buffer.
	//
	// @param    username The user whose position is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWhere(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Where" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.Where)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "CreaturesInMap" message to the outgoing data buffer.
	//
	// @param    map The map in which to check for the existing creatures.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCreaturesInMap(Int32 Map)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CreaturesInMap" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CreaturesInMap)

		mOutgoingBuffer->WriteInt16(Map);

	}

	//
	// Writes the "WarpMeToTarget" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWarpMeToTarget()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "WarpMeToTarget" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.WarpMeToTarget)
	}

	//
	// Writes the "WarpChar" message to the outgoing data buffer.
	//
	// @param    username The user to be warped. "YO" represent//s the user//s char.
	// @param    map The map to which to warp the character.
	// @param    x The x position in the map to which to waro the character.
	// @param    y The y position in the map to which to waro the character.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWarpChar(String UserName, Int32 Map, UInt8 X, UInt8 Y)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "WarpChar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.WarpChar)

		//Call.WriteString(UserName)

		mOutgoingBuffer->WriteInt16(Map);

		mOutgoingBuffer->WriteUInt8(X);
		mOutgoingBuffer->WriteUInt8(Y);

	}

	//
	// Writes the "Silence" message to the outgoing data buffer.
	//
	// @param    username The user to silence.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSilence(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Silence" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.Silence)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "SOSShowList" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSOSShowList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SOSShowList" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.SOSShowList)
	}

	//
	// Writes the "SOSRemove" message to the outgoing data buffer.
	//
	// @param    username The user whose SOS has been already attended.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSOSRemove(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SOSRemove" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SOSRemove)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "GoToChar" message to the outgoing data buffer.
	//
	// @param    username The user to be approached.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGoToChar(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GoToChar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.GoToChar)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "invisible" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteInvisible()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "invisible" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.invisible)
	}

	//
	// Writes the "GMPanel" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGMPanel()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GMPanel" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.GMPanel)
	}

	//
	// Writes the "RequestUserList" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestUserList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestUserList" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.RequestUserList)
	}

	//
	// Writes the "Working" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWorking()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Working" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.Working)
	}

	//
	// Writes the "Hiding" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteHiding()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Hiding" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.Hiding)
	}

	//
	// Writes the "Jail" message to the outgoing data buffer.
	//
	// @param    username The user to be sent to jail.
	// @param    reason The reason for which to send him to jail.
	// @param    time The time (in minutes) the user will have to spend there.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteJail(String UserName, String Reason, UInt8 time)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Jail" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.Jail)

		//Call.WriteString(UserName)
		//Call.WriteString(Reason)

		mOutgoingBuffer->WriteUInt8(time);

	}

	//
	// Writes the "KillNPC" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteKillNPC()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "KillNPC" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.KillNPC)
	}

	//
	// Writes the "WarnUser" message to the outgoing data buffer.
	//
	// @param    username The user to be warned.
	// @param    reason Reason for the warning.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteWarnUser(String UserName, String Reason)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "WarnUser" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.WarnUser)

		//Call.WriteString(UserName)
		//Call.WriteString(Reason)

	}

	//
	// Writes the "EditChar" message to the outgoing data buffer.
	//
	// @param    UserName    The user to be edited.
	// @param    editOption  Indicates what to edit in the char.
	// @param    arg1        Additional argument 1. Contents depend on editoption.
	// @param    arg2        Additional argument 2. Contents depend on editoption.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteEditChar(String UserName, UInt8 EditOption, String arg1, String arg2)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "EditChar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.EditChar)

		//Call.WriteString(UserName)

		mOutgoingBuffer->WriteUInt8(EditOption);

		//Call.WriteString(arg1)
		//Call.WriteString(arg2)

	}

	//
	// Writes the "RequestCharInfo" message to the outgoing data buffer.
	//
	// @param    username The user whose information is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestCharInfo(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestCharInfo" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RequestCharInfo)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "RequestCharStats" message to the outgoing data buffer.
	//
	// @param    username The user whose stats are requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestCharStats(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestCharStats" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RequestCharStats)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "RequestCharGold" message to the outgoing data buffer.
	//
	// @param    username The user whose gold is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestCharGold(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestCharGold" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RequestCharGold)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "RequestCharInventory" message to the outgoing data buffer.
	//
	// @param    username The user whose inventory is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestCharInventory(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestCharInventory" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RequestCharInventory)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "RequestCharBank" message to the outgoing data buffer.
	//
	// @param    username The user whose banking information is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestCharBank(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestCharBank" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RequestCharBank)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "RequestCharSkills" message to the outgoing data buffer.
	//
	// @param    username The user whose skills are requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestCharSkills(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestCharSkills" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RequestCharSkills)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "ReviveChar" message to the outgoing data buffer.
	//
	// @param    username The user to eb revived.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteReviveChar(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ReviveChar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ReviveChar)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "OnlineGM" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteOnlineGM()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "OnlineGM" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.OnlineGM)
	}

	//
	// Writes the "OnlineMap" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteOnlineMap(Int32 Map)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 26/03/2009
		//Writes the "OnlineMap" message to the outgoing data buffer
		//26/03/2009: Now you don//t need to be in the map to use the comand, so you send the map to server
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.OnlineMap)

		mOutgoingBuffer->WriteInt16(Map);

	}

	//
	// Writes the "Forgive" message to the outgoing data buffer.
	//
	// @param    username The user to be forgiven.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteForgive(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Forgive" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.Forgive)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "Kick" message to the outgoing data buffer.
	//
	// @param    username The user to be kicked.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteKick(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Kick" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.Kick)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "Execute" message to the outgoing data buffer.
	//
	// @param    username The user to be executed.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteExecute(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Execute" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.Execute)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "BanChar" message to the outgoing data buffer.
	//
	// @param    username The user to be banned.
	// @param    reason The reson for which the user is to be banned.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBanChar(String UserName, String Reason)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BanChar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.banChar)

		//Call.WriteString(UserName)

		//Call.WriteString(Reason)

	}

	//
	// Writes the "UnbanChar" message to the outgoing data buffer.
	//
	// @param    username The user to be unbanned.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUnbanChar(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "UnbanChar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.UnbanChar)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "NPCFollow" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteNPCFollow()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "NPCFollow" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.NPCFollow)
	}

	//
	// Writes the "SummonChar" message to the outgoing data buffer.
	//
	// @param    username The user to be summoned.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSummonChar(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SummonChar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SummonChar)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "SpawnListRequest" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSpawnListRequest()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SpawnListRequest" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.SpawnListRequest)
	}

	//
	// Writes the "SpawnCreature" message to the outgoing data buffer.
	//
	// @param    creatureIndex The index of the creature in the spawn list to be spawned.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSpawnCreature(Int32 creatureIndex)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SpawnCreature" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SpawnCreature)

		mOutgoingBuffer->WriteInt16(creatureIndex);

	}

	//
	// Writes the "ResetNPCInventory" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteResetNPCInventory()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ResetNPCInventory" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ResetNPCInventory)
	}

	//
	// Writes the "CleanWorld" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCleanWorld()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CleanWorld" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.CleanWorld)
	}

	//
	// Writes the "ServerMessage" message to the outgoing data buffer.
	//
	// @param    message The message to be sent to players.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteServerMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ServerMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ServerMessage)

		//Call.WriteString(Message)

	}
	//
	// Writes the "MapMessage" message to the outgoing data buffer.
	//
	// @param    message The message to be sent to players.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteMapMessage(String Message)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 14/11/2010
		//Writes the "MapMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.MapMessage)

		//Call.WriteString(Message)

	}

	//
	// Writes the "NickToIP" message to the outgoing data buffer.
	//
	// @param    username The user whose IP is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteNickToIP(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "NickToIP" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.nickToIP)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "IPToNick" message to the outgoing data buffer.
	//
	// @param    IP The IP for which to search for players. Must be an array of 4 elements with the 4 components of the IP.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteIPToNick()//ByRef Ip() As Byte)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "IPToNick" message to the outgoing data buffer
		//***************************************************
		//If UBound(Ip()) - LBound(Ip()) + 1 < > 4 Then Exit Sub   //Invalid IP

		  //  DimInt32 i


		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.IPToNick)

		/*For i = LBound(Ip()) To UBound(Ip())
		mOutgoingBuffer->WriteUInt8(Ip(i))
		Next i*/

	}

	//
	// Writes the "GuildOnlineMembers" message to the outgoing data buffer.
	//
	// @param    guild The guild whose online player list is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildOnlineMembers(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildOnlineMembers" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.GuildOnlineMembers)

		//Call.WriteString(guild)

	}

	//
	// Writes the "TeleportCreate" message to the outgoing data buffer.
	//
	// @param    map the map to which the teleport will lead.
	// @param    x The position in the x axis to which the teleport will lead.
	// @param    y The position in the y axis to which the teleport will lead.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTeleportCreate(Int32 Map, UInt8 X, UInt8 Y, UInt8 Radio)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "TeleportCreate" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.TeleportCreate)

		mOutgoingBuffer->WriteInt16(Map);

		mOutgoingBuffer->WriteUInt8(X);
		mOutgoingBuffer->WriteUInt8(Y);

		mOutgoingBuffer->WriteUInt8(Radio);

	}

	//
	// Writes the "TeleportDestroy" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTeleportDestroy()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "TeleportDestroy" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.TeleportDestroy)
	}

	//
	// Writes the "RainToggle" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRainToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RainToggle" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.RainToggle)
	}

	//
	// Writes the "SetCharDescription" message to the outgoing data buffer.
	//
	// @param    desc The description to set to players.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSetCharDescription(String Desc)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SetCharDescription" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SetCharDescription)

		//Call.WriteString(Desc)

	}

	//
	// Writes the "ForceMIDIToMap" message to the outgoing data buffer.
	//
	// @param    midiID The ID of the midi file to play.
	// @param    map The map in which to play the given midi.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteForceMIDIToMap(UInt8 midiID, Int32 Map)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ForceMIDIToMap" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ForceMIDIToMap)

		mOutgoingBuffer->WriteUInt8(midiID);

		mOutgoingBuffer->WriteInt16(Map);

	}

	//
	// Writes the "ForceWAVEToMap" message to the outgoing data buffer.
	//
	// @param    waveID  The ID of the wave file to play.
	// @param    Map     The map into which to play the given wave.
	// @param    x       The position in the x axis in which to play the given wave.
	// @param    y       The position in the y axis in which to play the given wave.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteForceWAVEToMap(UInt8 waveID, Int32 Map, UInt8 X, UInt8 Y)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ForceWAVEToMap" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ForceWAVEToMap)

		mOutgoingBuffer->WriteUInt8(waveID);

		mOutgoingBuffer->WriteInt16(Map);

		mOutgoingBuffer->WriteUInt8(X);
		mOutgoingBuffer->WriteUInt8(Y);

	}

	//
	// Writes the "RoyalArmyMessage" message to the outgoing data buffer.
	//
	// @param    message The message to send to the royal army members.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRoyalArmyMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RoyalArmyMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RoyalArmyMessage)

		//Call.WriteString(Message)

	}

	//
	// Writes the "ChaosLegionMessage" message to the outgoing data buffer.
	//
	// @param    message The message to send to the chaos legion member.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChaosLegionMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChaosLegionMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChaosLegionMessage)

		//Call.WriteString(Message)

	}

	//
	// Writes the "CitizenMessage" message to the outgoing data buffer.
	//
	// @param    message The message to send to citizens.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCitizenMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CitizenMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CitizenMessage)

		//Call.WriteString(Message)

	}

	//
	// Writes the "CriminalMessage" message to the outgoing data buffer.
	//
	// @param    message The message to send to criminals.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCriminalMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CriminalMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CriminalMessage)

		//Call.WriteString(Message)

	}

	//
	// Writes the "TalkAsNPC" message to the outgoing data buffer.
	//
	// @param    message The message to send to the royal army members.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTalkAsNPC(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "TalkAsNPC" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.TalkAsNPC)

		//Call.WriteString(Message)

	}

	//
	// Writes the "DestroyAllItemsInArea" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteDestroyAllItemsInArea()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "DestroyAllItemsInArea" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.DestroyAllItemsInArea)
	}

	//
	// Writes the "AcceptRoyalCouncilMember" message to the outgoing data buffer.
	//
	// @param    username The name of the user to be accepted into the royal army council.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteAcceptRoyalCouncilMember(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "AcceptRoyalCouncilMember" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.AcceptRoyalCouncilMember)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "AcceptChaosCouncilMember" message to the outgoing data buffer.
	//
	// @param    username The name of the user to be accepted as a chaos council member.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteAcceptChaosCouncilMember(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "AcceptChaosCouncilMember" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.AcceptChaosCouncilMember)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "ItemsInTheFloor" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteItemsInTheFloor()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ItemsInTheFloor" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ItemsInTheFloor)
	}

	//
	// Writes the "MakeDumb" message to the outgoing data buffer.
	//
	// @param    username The name of the user to be made dumb.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteMakeDumb(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "MakeDumb" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.MakeDumb)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "MakeDumbNoMore" message to the outgoing data buffer.
	//
	// @param    username The name of the user who will no longer be dumb.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteMakeDumbNoMore(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "MakeDumbNoMore" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.MakeDumbNoMore)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "DumpIPTables" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteDumpIPTables()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "DumpIPTables" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.dumpIPTables)
	}

	//
	// Writes the "CouncilKick" message to the outgoing data buffer.
	//
	// @param    username The name of the user to be kicked from the council.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCouncilKick(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CouncilKick" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CouncilKick)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "SetTrigger" message to the outgoing data buffer.
	//
	// @param    trigger The type of trigger to be set to the tile.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSetTrigger(UInt8 Trigger)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SetTrigger" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SetTrigger)

		mOutgoingBuffer->WriteUInt8(Trigger);

	}

	//
	// Writes the "AskTrigger" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteAskTrigger()
	{
		//***************************************************
		//Author: Nicolas Matias Gonzalez (NIGO)
		//Last Modification: 04/13/07
		//Writes the "AskTrigger" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.AskTrigger)
	}

	//
	// Writes the "BannedIPList" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBannedIPList()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BannedIPList" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.BannedIPList)
	}

	//
	// Writes the "BannedIPReload" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBannedIPReload()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BannedIPReload" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.BannedIPReload)
	}

	//
	// Writes the "GuildBan" message to the outgoing data buffer.
	//
	// @param    guild The guild whose members will be banned.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteGuildBan(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "GuildBan" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.GuildBan)

		//Call.WriteString(guild)

	}

	//
	// Writes the "BanIP" message to the outgoing data buffer.
	//
	// @param    byIp    If set to true, we are banning by IP, otherwise the ip of a given character.
	// @param    IP      The IP for which to search for players. Must be an array of 4 elements with the 4 components of the IP.
	// @param    nick    The nick of the player whose ip will be banned.
	// @param    reason  The reason for the ban.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteBanIP(Bool byIp, String Nick, String Reason)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "BanIP" message to the outgoing data buffer
		//***************************************************
		//If byIp And UBound(Ip()) - LBound(Ip()) + 1 < > 4 Then Exit Sub   //Invalid IP

			//DimInt32 i


		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.BanIP)

		mOutgoingBuffer->WriteUInt8(byIp);

		/*If byIp Then
		For i = LBound(Ip()) To UBound(Ip())
		mOutgoingBuffer->WriteUInt8(Ip(i))
		Next i
		Else*/
		//Call.WriteString(Nick)

		//Call.WriteString(Reason)

	}

	//
	// Writes the "UnbanIP" message to the outgoing data buffer.
	//
	// @param    IP The IP for which to search for players. Must be an array of 4 elements with the 4 components of the IP.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteUnbanIP()//(ByRef Ip() As Byte)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "UnbanIP" message to the outgoing data buffer
		//***************************************************
		//If UBound(Ip()) - LBound(Ip()) + 1 < > 4 Then Exit Sub   //Invalid IP

			//DimInt32 i


		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.UnbanIP)

		/*For i = LBound(Ip()) To UBound(Ip())
		mOutgoingBuffer->WriteUInt8(Ip(i))
		Next i*/

	}

	//
	// Writes the "CreateItem" message to the outgoing data buffer.
	//
	// @param    itemIndex The index of the item to be created.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCreateItem(Int32 ItemIndex)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CreateItem" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CreateItem)
		mOutgoingBuffer->WriteInt16(ItemIndex);

	}

	//
	// Writes the "DestroyItems" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteDestroyItems()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "DestroyItems" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.DestroyItems)
	}

	//
	// Writes the "ChaosLegionKick" message to the outgoing data buffer.
	//
	// @param    username The name of the user to be kicked from the Chaos Legion.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChaosLegionKick(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChaosLegionKick" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChaosLegionKick)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "RoyalArmyKick" message to the outgoing data buffer.
	//
	// @param    username The name of the user to be kicked from the Royal Army.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRoyalArmyKick(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RoyalArmyKick" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RoyalArmyKick)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "ForceMIDIAll" message to the outgoing data buffer.
	//
	// @param    midiID The id of the midi file to play.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteForceMIDIAll(UInt8 midiID)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ForceMIDIAll" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ForceMIDIAll)

		mOutgoingBuffer->WriteUInt8(midiID);

	}

	//
	// Writes the "ForceWAVEAll" message to the outgoing data buffer.
	//
	// @param    waveID The id of the wave file to play.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteForceWAVEAll(UInt8 waveID)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ForceWAVEAll" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ForceWAVEAll)

		mOutgoingBuffer->WriteUInt8(waveID);

	}

	//
	// Writes the "RemovePunishment" message to the outgoing data buffer.
	//
	// @param    username The user whose punishments will be altered.
	// @param    punishment The id of the punishment to be removed.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRemovePunishment(String UserName, UInt8 punishment, String NewText)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RemovePunishment" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RemovePunishment)

		//Call.WriteString(UserName)
		mOutgoingBuffer->WriteUInt8(punishment);
		//Call.WriteString(NewText)

	}

	//
	// Writes the "TileBlockedToggle" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTileBlockedToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "TileBlockedToggle" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.TileBlockedToggle)
	}

	//
	// Writes the "KillNPCNoRespawn" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteKillNPCNoRespawn()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "KillNPCNoRespawn" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.KillNPCNoRespawn)
	}

	//
	// Writes the "KillAllNearbyNPCs" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteKillAllNearbyNPCs()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "KillAllNearbyNPCs" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.KillAllNearbyNPCs)
	}

	//
	// Writes the "LastIP" message to the outgoing data buffer.
	//
	// @param    username The user whose last IPs are requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteLastIP(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "LastIP" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.LastIP)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "ChangeMOTD" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMOTD()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChangeMOTD" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ChangeMOTD)
	}

	//
	// Writes the "SetMOTD" message to the outgoing data buffer.
	//
	// @param    message The message to be set as the new MOTD.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSetMOTD(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SetMOTD" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SetMOTD)

		//Call.WriteString(Message)

	}

	//
	// Writes the "SystemMessage" message to the outgoing data buffer.
	//
	// @param    message The message to be sent to all players.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSystemMessage(String Message)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SystemMessage" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SystemMessage)

		//Call.WriteString(Message)

	}

	//
	// Writes the "CreateNPC" message to the outgoing data buffer.
	//
	// @param    npcIndex The index of the NPC to be created.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCreateNPC(Int32 NPCIndex)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CreateNPC" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CreateNPC)

		mOutgoingBuffer->WriteInt16(NPCIndex);

	}

	//
	// Writes the "CreateNPCWithRespawn" message to the outgoing data buffer.
	//
	// @param    npcIndex The index of the NPC to be created.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCreateNPCWithRespawn(Int32 NPCIndex)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CreateNPCWithRespawn" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CreateNPCWithRespawn)

		mOutgoingBuffer->WriteInt16(NPCIndex);
	}

	//
	// Writes the "ImperialArmour" message to the outgoing data buffer.
	//
	// @param    armourIndex The index of imperial armour to be altered.
	// @param    objectIndex The index of the new object to be set as the imperial armour.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteImperialArmour(UInt8 armourIndex, Int32 objectIndex)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ImperialArmour" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ImperialArmour)

		mOutgoingBuffer->WriteUInt8(armourIndex);

		mOutgoingBuffer->WriteInt16(objectIndex);

	}

	//
	// Writes the "ChaosArmour" message to the outgoing data buffer.
	//
	// @param    armourIndex The index of chaos armour to be altered.
	// @param    objectIndex The index of the new object to be set as the chaos armour.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChaosArmour(UInt8 armourIndex, Int32 objectIndex)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChaosArmour" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChaosArmour)

		mOutgoingBuffer->WriteUInt8(armourIndex);

		mOutgoingBuffer->WriteInt16(objectIndex);

	}

	//
	// Writes the "NavigateToggle" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteNavigateToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "NavigateToggle" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.NavigateToggle)
	}

	//
	// Writes the "ServerOpenToUsersToggle" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteServerOpenToUsersToggle()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ServerOpenToUsersToggle" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ServerOpenToUsersToggle)
	}

	//
	// Writes the "TurnOffServer" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTurnOffServer()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "TurnOffServer" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.TurnOffServer)
	}

	//
	// Writes the "TurnCriminal" message to the outgoing data buffer.
	//
	// @param    username The name of the user to turn into criminal.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteTurnCriminal(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "TurnCriminal" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.TurnCriminal)

		//Call.WriteString(UserName)

	}

	//
	// Writes the "ResetFactions" message to the outgoing data buffer.
	//
	// @param    username The name of the user who will be removed from any faction.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteResetFactions(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ResetFactions" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		// mOutgoingBuffer->WriteUInt8(eGMCommands.ResetFactions)

		 //Call.WriteString(UserName)

	}

	//
	// Writes the "RemoveCharFromGuild" message to the outgoing data buffer.
	//
	// @param    username The name of the user who will be removed from any guild.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRemoveCharFromGuild(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RemoveCharFromGuild" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		// mOutgoingBuffer->WriteUInt8(eGMCommands.RemoveCharFromGuild)

		 //Call.WriteString(UserName)

	}

	//
	// Writes the "RequestCharMail" message to the outgoing data buffer.
	//
	// @param    username The name of the user whose mail is requested.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRequestCharMail(String UserName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "RequestCharMail" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		// mOutgoingBuffer->WriteUInt8(eGMCommands.RequestCharMail)

		 //Call.WriteString(UserName)

	}

	//
	// Writes the "AlterPassword" message to the outgoing data buffer.
	//
	// @param    username The name of the user whose mail is requested.
	// @param    copyFrom The name of the user from which to copy the password.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteAlterPassword(String UserName, String CopyFrom)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "AlterPassword" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.AlterPassword)

		//Call.WriteString(UserName)
		//Call.WriteString(CopyFrom)

	}

	//
	// Writes the "AlterMail" message to the outgoing data buffer.
	//
	// @param    username The name of the user whose mail is requested.
	// @param    newMail The new email of the player.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteAlterMail(String UserName, String newMail)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "AlterMail" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		// mOutgoingBuffer->WriteUInt8(eGMCommands.AlterMail)

		 //Call.WriteString(UserName)
		 //Call.WriteString(newMail)

	}

	//
	// Writes the "AlterName" message to the outgoing data buffer.
	//
	// @param    username The name of the user whose mail is requested.
	// @param    newName The new user name.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteAlterName(String UserName, String newName)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "AlterName" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.AlterName)

		//Call.WriteString(UserName)
		//Call.WriteString(newName)

	}

	//
	// Writes the "ToggleCentinelActivated" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteToggleCentinelActivated()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ToggleCentinelActivated" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ToggleCentinelActivated)
	}

	//
	// Writes the "DoBackup" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteDoBackup()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "DoBackup" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//    outgoingData.WriteByte(eGMCommands.DoBackUp)
	}

	//
	// Writes the "ShowGuildMessages" message to the outgoing data buffer.
	//
	// @param    guild The guild to listen to.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteShowGuildMessages(String guild)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ShowGuildMessages" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//  mOutgoingBuffer->WriteUInt8(eGMCommands.ShowGuildMessages)

		  //Call.WriteString(guild)

	}

	//
	// Writes the "SaveMap" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSaveMap()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SaveMap" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		// outgoingData.WriteByte(eGMCommands.SaveMap)
	}

	//
	// Writes the "ChangeMapInfoPK" message to the outgoing data buffer.
	//
	// @param    isPK True if the map is PK, False otherwise.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoPK(Bool isPK)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChangeMapInfoPK" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoPK)

		mOutgoingBuffer->WriteUInt8(isPK);

	}

	//
	// Writes the "ChangeMapInfoNoOcultar" message to the outgoing data buffer.
	//
	// @param    PermitirOcultar True if the map permits to hide, False otherwise.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoNoOcultar(Bool PermitirOcultar)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 19/09/2010
		//Writes the "ChangeMapInfoNoOcultar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		// mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoNoOcultar)

		mOutgoingBuffer->WriteUInt8(PermitirOcultar);

	}

	//
	// Writes the "ChangeMapInfoNoInvocar" message to the outgoing data buffer.
	//
	// @param    PermitirInvocar True if the map permits to invoke, False otherwise.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoNoInvocar(Bool PermitirInvocar)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 18/09/2010
		//Writes the "ChangeMapInfoNoInvocar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoNoInvocar)

		mOutgoingBuffer->WriteUInt8(PermitirInvocar);

	}

	//
	// Writes the "ChangeMapInfoBackup" message to the outgoing data buffer.
	//
	// @param    backup True if the map is to be backuped, False otherwise.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoBackup(Bool backup)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChangeMapInfoBackup" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoBackup)

		mOutgoingBuffer->WriteUInt8(backup);

	}

	//
	// Writes the "ChangeMapInfoRestricted" message to the outgoing data buffer.
	//
	// @param    restrict NEWBIES (only newbies), NO (everyone), ARMADA (just Armadas), CAOS (just caos) or FACCION (Armadas & caos only)
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoRestricted(String restrict)
	{
		//***************************************************
		//Author: Pablo (ToxicWaste)
		//Last Modification: 26/01/2007
		//Writes the "ChangeMapInfoRestricted" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoRestricted)

		//Call.WriteString(restrict)

	}

	//
	// Writes the "ChangeMapInfoNoMagic" message to the outgoing data buffer.
	//
	// @param    nomagic TRUE if no magic is to be allowed in the map.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoNoMagic(Bool nomagic)
	{
		//***************************************************
		//Author: Pablo (ToxicWaste)
		//Last Modification: 26/01/2007
		//Writes the "ChangeMapInfoNoMagic" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoNoMagic)

		mOutgoingBuffer->WriteUInt8(nomagic);

	}

	//
	// Writes the "ChangeMapInfoNoInvi" message to the outgoing data buffer.
	//
	// @param    noinvi TRUE if invisibility is not to be allowed in the map.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoNoInvi(Bool noinvi)
	{
		//***************************************************
		//Author: Pablo (ToxicWaste)
		//Last Modification: 26/01/2007
		//Writes the "ChangeMapInfoNoInvi" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoNoInvi)

		mOutgoingBuffer->WriteUInt8(noinvi);

	}

	//
	// Writes the "ChangeMapInfoNoResu" message to the outgoing data buffer.
	//
	// @param    noresu TRUE if resurection is not to be allowed in the map.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoNoResu(Bool noresu)
	{
		//***************************************************
		//Author: Pablo (ToxicWaste)
		//Last Modification: 26/01/2007
		//Writes the "ChangeMapInfoNoResu" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoNoResu)

		mOutgoingBuffer->WriteUInt8(noresu);

	}

	//
	// Writes the "ChangeMapInfoLand" message to the outgoing data buffer.
	//
	// @param    land options: "BOSQUE", "NIEVE", "DESIERTO", "CIUDAD", "CAMPO", "DUNGEON".
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoLand(String land)
	{
		//***************************************************
		//Author: Pablo (ToxicWaste)
		//Last Modification: 26/01/2007
		//Writes the "ChangeMapInfoLand" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoLand)

		//Call.WriteString(land)

	}

	//
	// Writes the "ChangeMapInfoZone" message to the outgoing data buffer.
	//
	// @param    zone options: "BOSQUE", "NIEVE", "DESIERTO", "CIUDAD", "CAMPO", "DUNGEON".
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoZone(String zone)
	{
		//***************************************************
		//Author: Pablo (ToxicWaste)
		//Last Modification: 26/01/2007
		//Writes the "ChangeMapInfoZone" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoZone)

		//Call.WriteString(zone)

	}

	//
	// Writes the "ChangeMapInfoStealNpc" message to the outgoing data buffer.
	//
	// @param    forbid TRUE if stealNpc forbiden.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChangeMapInfoStealNpc(Bool forbid)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 25/07/2010
		//Writes the "ChangeMapInfoStealNpc" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChangeMapInfoStealNpc)

		mOutgoingBuffer->WriteUInt8(forbid);

	}

	//
	// Writes the "SaveChars" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSaveChars()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "SaveChars" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.SaveChars)
	}

	//
	// Writes the "CleanSOS" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCleanSOS()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "CleanSOS" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.CleanSOS)
	}

	//
	// Writes the "ShowServerForm" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteShowServerForm()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ShowServerForm" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ShowServerForm)
	}

	//
	// Writes the "ShowDenouncesList" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteShowDenouncesList()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 14/11/2010
		//Writes the "ShowDenouncesList" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ShowDenouncesList)
	}

	//
	// Writes the "EnableDenounces" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteEnableDenounces()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 14/11/2010
		//Writes the "EnableDenounces" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.EnableDenounces)
	}

	//
	// Writes the "Night" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteNight()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Night" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.night)
	}

	//
	// Writes the "KickAllChars" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteKickAllChars()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "KickAllChars" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.KickAllChars)
	}

	//
	// Writes the "ReloadNPCs" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteReloadNPCs()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ReloadNPCs" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ReloadNPCs)
	}

	//
	// Writes the "ReloadServerIni" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteReloadServerIni()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ReloadServerIni" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ReloadServerIni)
	}

	//
	// Writes the "ReloadSpells" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteReloadSpells()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ReloadSpells" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ReloadSpells)
	}

	//
	// Writes the "ReloadObjects" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteReloadObjects()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ReloadObjects" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.ReloadObjects)
	}

	//
	// Writes the "Restart" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRestart()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Restart" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.Restart)
	}

	//
	// Writes the "ResetAutoUpdate" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteResetAutoUpdate()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ResetAutoUpdate" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		// outgoingData.WriteByte(eGMCommands.ResetAutoUpdate)
	}

	//
	// Writes the "ChatColor" message to the outgoing data buffer.
	//
	// @param    r The red component of the new chat color.
	// @param    g The green component of the new chat color.
	// @param    b The blue component of the new chat color.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteChatColor(UInt8 r, UInt8 g, UInt8 b)
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "ChatColor" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.ChatColor);

		mOutgoingBuffer->WriteUInt8(r);
		mOutgoingBuffer->WriteUInt8(g);
		mOutgoingBuffer->WriteUInt8(b);

	}

	//
	// Writes the "Ignored" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteIgnored()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 05/17/06
		//Writes the "Ignored" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.Ignored)
	}

	//
	// Writes the "CheckSlot" message to the outgoing data buffer.
	//
	// @param    UserName    The name of the char whose slot will be checked.
	// @param    slot        The slot to be checked.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCheckSlot(String UserName, UInt8 slot)
	{
		//***************************************************
		//Author: Pablo (ToxicWaste)
		//Last Modification: 26/01/2007
		//Writes the "CheckSlot" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CheckSlot)
		//Call.WriteString(UserName)
		mOutgoingBuffer->WriteUInt8(slot);

	}

	//
	// Writes the "Ping" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WritePing()
	{
		//***************************************************
		//Author: Juan Martín Sotuyo Dodero (Maraxus)
		//Last Modification: 26/01/2007
		//Writes the "Ping" message to the outgoing data buffer
		//***************************************************
			//Prevent the timer from being cut
	   // If pingTime < > 0 Then Exit Sub

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::Ping));

		// Avoid Protocol:: computing errors due to frame rate
	   // FlushBuffer
	   // DoEvents

	   // pingTime = GetTickCount
	}

	//
	// Writes the "ShareNpc" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteShareNpc()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 15/04/2010
		//Writes the "ShareNpc" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::ShareNpc));
	}

	//
	// Writes the "StopSharingNpc" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteStopSharingNpc()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 15/04/2010
		//Writes the "StopSharingNpc" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::StopSharingNpc));
	}

	//
	// Writes the "SetIniVar" message to the outgoing data buffer.
	//
	// @param    sLlave the name of the key which contains the value to edit
	// @param    sClave the name of the value to edit
	// @param    sValor the new value to set to sClave
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteSetIniVar(String sLlave, String sClave, String sValor)
	{
		//***************************************************
		//Author: Brian Chaia (BrianPr)
		//Last Modification: 21/06/2009
		//Writes the "SetIniVar" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SetIniVar)

		//Call.WriteString(sLlave)
		//Call.WriteString(sClave)
		//Call.WriteString(sValor)

	}

	//
	// Writes the "CreatePretorianClan" message to the outgoing data buffer.
	//
	// @param    Map         The map in which create the pretorian clan.
	// @param    X           The x pos where the king is settled.
	// @param    Y           The y pos where the king is settled.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteCreatePretorianClan(Int32 Map, UInt8 X, UInt8 Y)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 29/10/2010
		//Writes the "CreatePretorianClan" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.CreatePretorianClan)
		mOutgoingBuffer->WriteInt16(Map);
		mOutgoingBuffer->WriteUInt8(X);
		mOutgoingBuffer->WriteUInt8(Y);

	}

	//
	// Writes the "DeletePretorianClan" message to the outgoing data buffer.
	//
	// @param    Map         The map which contains the pretorian clan to be removed.
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteDeletePretorianClan(Int32 Map)
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 29/10/2010
		//Writes the "DeletePretorianClan" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RemovePretorianClan)
		mOutgoingBuffer->WriteInt16(Map);

	}

	void Protocol::WriteSetDialog(String dialog)
	{
		//***************************************************
		//Author: Amraphen
		//Last Modification: 18/11/2010
		//Writes the "SetDialog" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.SetDialog)

		//Call.WriteString(dialog)

	}

	//
	// Writes the "Impersonate" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteImpersonate()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 20/11/2010
		//Writes the "Impersonate" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.Impersonate)
	}

	//
	// Writes the "Imitate" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteImitate()
	{
		//***************************************************
		//Author: ZaMa
		//Last Modification: 20/11/2010
		//Writes the "Imitate" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.Imitate)
	}

	//
	// Writes the "RecordAddObs" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRecordAddObs(UInt8 RecordIndex, String Observation)
	{
		//***************************************************
		//Author: Amraphen
		//Last Modification: 29/11/2010
		//Writes the "RecordAddObs" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RecordAddObs)

		mOutgoingBuffer->WriteUInt8(RecordIndex);
		//Call.WriteString(Observation)

	}

	//
	// Writes the "RecordAdd" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRecordAdd(String Nickname, String Reason)
	{
		//***************************************************
		//Author: Amraphen
		//Last Modification: 29/11/2010
		//Writes the "RecordAdd" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RecordAdd)

		//Call.WriteString(Nickname)
		//Call.WriteString(Reason)

	}

	//
	// Writes the "RecordRemove" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRecordRemove(UInt8 RecordIndex)
	{
		//***************************************************
		//Author: Amraphen
		//Last Modification: 29/11/2010
		//Writes the "RecordRemove" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RecordRemove)

		mOutgoingBuffer->WriteUInt8(RecordIndex);

	}

	//
	// Writes the "RecordListRequest" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRecordListRequest()
	{
		//***************************************************
		//Author: Amraphen
		//Last Modification: 29/11/2010
		//Writes the "RecordListRequest" message to the outgoing data buffer
		//***************************************************
		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//outgoingData.WriteByte(eGMCommands.RecordListRequest)
	}

	//
	// Writes the "RecordDetailsRequest" message to the outgoing data buffer.
	//
	// @remarks  The data is not actually sent until the buffer is properly flushed.

	void Protocol::WriteRecordDetailsRequest(UInt8 RecordIndex)
	{
		//***************************************************
		//Author: Amraphen
		//Last Modification: 29/11/2010
		//Writes the "RecordDetailsRequest" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::GMCommands));
		//mOutgoingBuffer->WriteUInt8(eGMCommands.RecordDetailsRequest)

		mOutgoingBuffer->WriteUInt8(RecordIndex);

	}

	//
	// Handles the RecordList message.

	void Protocol::HandleRecordList()
	{
		//***************************************************
		//Author: Amraphen
		//Last Modification: 29/11/2010
		//
		//***************************************************
	/*
			DimUInt8 NumRecords
			DimInt32 i

			NumRecords = Buffer.ReadByte

			//Se limpia el ListBox y se agregan los usuarios
			frmPanelGm.lstUsers.Clear
			For i = 1 To NumRecords
			frmPanelGm.lstUsers.AddItem Buffer.ReadASCIIString
			Next i
			*/
	}

	//
	// Handles the RecordDetails message.

	void Protocol::HandleRecordDetails()
	{
		//***************************************************
		//Author: Amraphen
		//Last Modification: 29/11/2010
		//
		//***************************************************

		/*
			With frmPanelGm
			.txtCreador.Text = Buffer.ReadASCIIString
			.txtDescrip.Text = Buffer.ReadASCIIString

			//Status del pj
			If Buffer.ReadBoolean Then
			.lblEstado.ForeColor = vbGreen
			.lblEstado.Caption = "ONLINE"
			Else
			.lblEstado.ForeColor = vbRed
			.lblEstado.Caption = "OFFLINE"
			End If

			//IP del personaje
			tmpStr = Buffer.ReadASCIIString
			If LenB(tmpStr) Then
			.txtIP.Text = tmpStr
			Else
			.txtIP.Text = "Usuario offline"
			End If

			//Tiempo online
			tmpStr = Buffer.ReadASCIIString
			If LenB(tmpStr) Then
			.txtTimeOn.Text = tmpStr
			Else
			.txtTimeOn.Text = "Usuario offline"
			End If

			//Observaciones
			tmpStr = Buffer.ReadASCIIString
			If LenB(tmpStr) Then
			.txtObs.Text = tmpStr
			Else
			.txtObs.Text = "Sin observaciones"
			End If
			*/
	}

	//
	// Writes the "Moveitem" message to the outgoing data buffer.
	//
	void Protocol::WriteMoveItem(Int32 originalSlot, Int32 newSlot, UInt8 moveType)
	{
		//***************************************************
		//Author: Budi
		//Last Modification: 05/01/2011
		//Writes the "MoveItem" message to the outgoing data buffer
		//***************************************************

		mOutgoingBuffer->WriteUInt8(static_cast<UInt8>(ClientPacketID::MoveItem));
		mOutgoingBuffer->WriteUInt8(originalSlot);
		mOutgoingBuffer->WriteUInt8(newSlot);
		mOutgoingBuffer->WriteUInt8(moveType);

	}
}