
#include "api/trader/traderspi.h"

extern int requestId;


char MapDirection(char src, bool toOrig);
char MapOffset(char src, bool toOrig);


void CtpTraderSpi::OnFrontConnected()
{
	cerr<<" 连接交易前置.trader..成功"<<endl;
}

void CtpTraderSpi::ReqUserLogin(TThostFtdcBrokerIDType	vAPPID,
	        TThostFtdcUserIDType	vUserId,	TThostFtdcPasswordType	vPasswd)
{

	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, vAPPID); strcpy(APPID, vAPPID);
	strcpy(req.UserID, vUserId);  strcpy(USERID, vUserId);
	strcpy(req.Password, vPasswd);
	int ret = pUserApi->ReqUserLogin(&req, ++requestId);
  cerr<<" 请求 | 发送登录..."<<((ret == 0) ? "成功" :"失败") << endl;
}

void CtpTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if ( !IsErrorRspInfo(pRspInfo) && pRspUserLogin ) {
    // 保存会话参数
		frontId = pRspUserLogin->FrontID;
		sessionId = pRspUserLogin->SessionID;
		int nextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		sprintf(orderRef, "%d", ++nextOrderRef);
    cerr<<" 响应 | 登录成功...当前交易日:"
      <<pRspUserLogin->TradingDay<<endl;
  }
}

void CtpTraderSpi::ReqQrySettlementInfo()
{
	CThostFtdcQrySettlementInfoField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, APPID);
	strcpy(req.InvestorID, USERID);
	int ret = pUserApi->ReqQrySettlementInfo(&req, ++requestId);
	cerr<<" 请求 | 发送结算结果查询..."<<((ret == 0)?"成功":"失败")<<endl;
}

void CtpTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, APPID);
	strcpy(req.InvestorID, USERID);
	int ret = pUserApi->ReqSettlementInfoConfirm(&req, ++requestId);
	cerr<<" 请求 | 发送结算单确认..."<<((ret == 0)?"成功":"失败")<<endl;
}

void CtpTraderSpi::OnRspSettlementInfoConfirm(
        CThostFtdcSettlementInfoConfirmField  *pSettlementInfoConfirm,
        CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cout<<"the function is called"<<endl;
	if( !IsErrorRspInfo(pRspInfo) && pSettlementInfoConfirm){
    cerr<<" 响应 | 结算单..."<<pSettlementInfoConfirm->InvestorID
      <<"...<"<<pSettlementInfoConfirm->ConfirmDate
      <<" "<<pSettlementInfoConfirm->ConfirmTime<<">...确认"<<endl;
    }
    else{
       cout<<"the selletment info confirm is wrong"<<endl;
    }
}

void CtpTraderSpi::ReqQryInstrument(TThostFtdcInstrumentIDType instId)
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
   strcpy(req.InstrumentID, instId);//为空表示查询所有合约
	int ret = pUserApi->ReqQryInstrument(&req, ++requestId);
	cerr<<" 请求 | 发送合约查询..."<<((ret == 0)?"成功":"失败")<<endl;
}

void CtpTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument,
         CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cout<<"this OnRspQryInstrument is called"<<endl;
	if ( !IsErrorRspInfo(pRspInfo) &&  pInstrument){
    cerr<<" 响应 | 合约:"<<pInstrument->InstrumentID
      <<" 交割月:"<<pInstrument->DeliveryMonth
      <<" 多头保证金率:"<<pInstrument->LongMarginRatio
      <<" 空头保证金率:"<<pInstrument->ShortMarginRatio<<endl;
  }
}

void CtpTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, APPID);
	strcpy(req.InvestorID, USERID);
	int ret = pUserApi->ReqQryTradingAccount(&req, ++requestId);
	cerr<<" 请求 | 发送资金查询..."<<((ret == 0)?"成功":"失败")<<endl;

}

void CtpTraderSpi::OnRspQryTradingAccount(
    CThostFtdcTradingAccountField *pTradingAccount,
   CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cout<<"this OnRspQryTradingAccount is called"<<endl;
	if (!IsErrorRspInfo(pRspInfo) &&  pTradingAccount){
    cerr<<" 响应 | 权益:"<<pTradingAccount->Balance
      <<" 可用:"<<pTradingAccount->Available
      <<" 保证金:"<<pTradingAccount->CurrMargin
      <<" 平仓盈亏:"<<pTradingAccount->CloseProfit
      <<" 持仓盈亏"<<pTradingAccount->PositionProfit
      <<" 手续费:"<<pTradingAccount->Commission
      <<" 冻结保证金:"<<pTradingAccount->FrozenMargin
      //<<" 冻结手续费:"<<pTradingAccount->FrozenCommission
      << endl;
  }
}

void CtpTraderSpi::ReqQryInvestorPosition(TThostFtdcInstrumentIDType instId)
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, APPID);
	strcpy(req.InvestorID, USERID);
	strcpy(req.InstrumentID, instId);
	int ret = pUserApi->ReqQryInvestorPosition(&req, ++requestId);
	cerr<<" 请求 | 发送持仓查询..."<<((ret == 0)?"成功":"失败")<<endl;
}

void CtpTraderSpi::OnRspQryInvestorPosition(
    CThostFtdcInvestorPositionField *pInvestorPosition,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
   cout<<"this OnRspQryInvestorPosition is called"<<endl;
  if( !IsErrorRspInfo(pRspInfo) &&  pInvestorPosition ){
    cerr<<" 响应 | 合约:"<<pInvestorPosition->InstrumentID
      <<" 方向:"<<MapDirection(pInvestorPosition->PosiDirection-2,false)
      <<" 总持仓:"<<pInvestorPosition->Position
      <<" 昨仓:"<<pInvestorPosition->YdPosition
      <<" 今仓:"<<pInvestorPosition->TodayPosition
      <<" 持仓盈亏:"<<pInvestorPosition->PositionProfit
      <<" 保证金:"<<pInvestorPosition->UseMargin<<endl;
  }
}

void CtpTraderSpi::ReqOrderInsert(TThostFtdcInstrumentIDType instId,
    TThostFtdcDirectionType dir,TThostFtdcPriceType price, TThostFtdcVolumeType vol)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, APPID);  //应用单元代码
	strcpy(req.InvestorID, USERID); //投资者代码
	strcpy(req.InstrumentID, instId); //合约代码
	strcpy(req.OrderRef, orderRef);  //报单引用
  int nextOrderRef = atoi(orderRef);
  sprintf(orderRef, "%d", ++nextOrderRef);

  req.LimitPrice = price;	//价格
  if(0==req.LimitPrice){
	  req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;//价格类型=市价
	  req.TimeCondition = THOST_FTDC_TC_IOC;//有效期类型:立即完成，否则撤销
  }else{
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//价格类型=限价
    req.TimeCondition = THOST_FTDC_TC_GFD;  //有效期类型:当日有效
  }
  req.Direction = MapDirection(dir,true);  //买卖方向
	//req.CombOffsetFlag[0] = MapOffset(kpp[0],true); //组合开平标志:开仓
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志
	req.VolumeTotalOriginal = vol;	///数量
	req.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
	req.MinVolume = 1;	//最小成交量:1
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即

  //TThostFtdcPriceType	StopPrice;  //止损价
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平
	req.IsAutoSuspend = 0;  //自动挂起标志:否
	req.UserForceClose = 0;   //用户强评标志:否

	int ret = pUserApi->ReqOrderInsert(&req, ++requestId);
	cerr<<" 请求 | 发送报单..."<<((ret == 0)?"成功":"失败")<< endl;
}


void CtpTraderSpi::ReqOrderInsertTake(TThostFtdcInstrumentIDType instId,
    TThostFtdcDirectionType dir, TThostFtdcVolumeType vol)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, APPID);  //应用单元代码
	strcpy(req.InvestorID, USERID); //投资者代码
	strcpy(req.InstrumentID, instId); //合约代码
	strcpy(req.OrderRef, orderRef);  //报单引用
  int nextOrderRef = atoi(orderRef);
  sprintf(orderRef, "%d", ++nextOrderRef);

	  req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;//价格类型=市价
	  //req.TimeCondition = THOST_FTDC_TC_IOC;//有效期类型:立即完成，否则撤销
  req.Direction = MapDirection(dir,true);  //买卖方向
	//req.CombOffsetFlag[0] = MapOffset(kpp[0],true); //组合开平标志:开仓
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志
	req.VolumeTotalOriginal = vol;	///数量
	req.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
	req.MinVolume = 1;	//最小成交量:1
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即

  //TThostFtdcPriceType	StopPrice;  //止损价
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平
	req.IsAutoSuspend = 0;  //自动挂起标志:否
	req.UserForceClose = 0;   //用户强评标志:否

	int ret = pUserApi->ReqOrderInsert(&req, ++requestId);
	cerr<<" 请求 | 发送报单..."<<((ret == 0)?"成功":"失败")<< endl;
}

void CtpTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
          CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
   cout<<"this OnRspOrderInsert is called"<<endl;
  if( !IsErrorRspInfo(pRspInfo) && pInputOrder ){
    cerr<<"响应 | 报单提交成功...报单引用:"<<pInputOrder->OrderRef<<endl;
  }
}

void CtpTraderSpi::ReqOrderAction(TThostFtdcSequenceNoType orderSeq)
{
  bool found=false; unsigned int i=0;
  for(i=0;i<orderList.size();i++){
    if(orderList[i]->BrokerOrderSeq == orderSeq){ found = true; break;}
  }
  if(!found){cerr<<" 请求 | 报单不存在."<<endl; return;}

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, APPID);   //经纪公司代码
	strcpy(req.InvestorID, USERID); //投资者代码
	//strcpy(req.OrderRef, pOrderRef); //报单引用
	//req.FrontID = frontId;           //前置编号
	//req.SessionID = sessionId;       //会话编号
  strcpy(req.ExchangeID, orderList[i]->ExchangeID);
  strcpy(req.OrderSysID, orderList[i]->OrderSysID);
	req.ActionFlag = THOST_FTDC_AF_Delete;  //操作标志

	int ret = pUserApi->ReqOrderAction(&req, ++requestId);
	cerr<< " 请求 | 发送撤单..." <<((ret == 0)?"成功":"失败") << endl;
}

void CtpTraderSpi::OnRspOrderAction(
      CThostFtdcInputOrderActionField *pInputOrderAction,
      CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  cout<<"this OnRspOrderAction is called"<<endl;
  if (!IsErrorRspInfo(pRspInfo) && pInputOrderAction){
    cerr<< " 响应 | 撤单成功..."
      << "交易所:"<<pInputOrderAction->ExchangeID
      <<" 报单编号:"<<pInputOrderAction->OrderSysID<<endl;
  }
}

bool CtpTraderSpi::CheckToLock(TThostFtdcInstrumentIDType InstrumentID,TThostFtdcPriceType lastPrice){
  string tmpId = InstrumentID;
  vector<CThostFtdcTradeField*> tradeVector = tradeMapVector[tmpId];
  int sell = 0;
  int buy = 0;
  for(int i=0;i<tradeVector.size();i++){
    if(tradeVector[i]->Direction ==THOST_FTDC_D_Buy && tradeVector[i]->OffsetFlag ==THOST_FTDC_OF_Open){
      buy = buy+tradeVector[i]->Volume;
    }
    else if(tradeVector[i]->Direction ==THOST_FTDC_D_Sell && tradeVector[i]->OffsetFlag ==THOST_FTDC_OF_Open){
      sell = sell+tradeVector[i]->Volume;
    }
  }
  if (sell ==buy){
    //表示已经锁仓了，已经不需要锁仓了。
    return true;
  }
  else if (sell>buy){
    //表示卖的比买的多，所以还是需要锁仓的，需要在买差值。
    //锁仓采用的方法是按照市价，立即成交，否则撤单的方式
    cout<<"调用锁仓函数"<<endl;
    TThostFtdcVolumeType num = sell-buy;
    ReqOrderInsert(InstrumentID,THOST_FTDC_D_Buy,lastPrice,num);
    return false;
  }
  else{
    //表示买的比卖的多，所以还是需要锁仓的，需要卖差值。
    //锁仓采用的方法是按照市价，立即成交，否则撤单的方式
    cout<<"调用锁仓函数"<<endl;
    TThostFtdcVolumeType num =buy -sell;
    ReqOrderInsertTake(InstrumentID,THOST_FTDC_D_Sell,num);
    return false;
  }
}

///报单回报
void CtpTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
  CThostFtdcOrderField* order = new CThostFtdcOrderField();
  memcpy(order,  pOrder, sizeof(CThostFtdcOrderField));
  bool founded=false;
  string instrumentID = order->InstrumentID;
  if(orderMapVector.find(instrumentID) != orderMapVector.end()){
    //说明此前有相应的合约编码的报单
    for(unsigned int i=0;i<orderMapVector[instrumentID].size();i++){
      if(orderMapVector[instrumentID][i]->BrokerOrderSeq == order->BrokerOrderSeq){
        CThostFtdcOrderField* tmp = orderMapVector[instrumentID][i];
        orderMapVector[instrumentID][i] = order;
        delete tmp;
        founded = true;
        break;
      }
    }
    if(!founded){
      orderMapVector[instrumentID].push_back(order);
    }
  }
  else{
    //第一次发现此合约编码类型的合约
    orderMapVector[instrumentID].push_back(order);
  }
  pthread_t tmp = pthread_self();
  cerr<<" 回报 | 报单已提交...序号:"<<order->BrokerOrderSeq<< " the thread is "<<tmp<<endl;
  //cerr<<" 回报 | 报单的状态是...序号:"<<order->StatusMsg<< " the thread is "<<tmp<<endl;
  basicPrint(order->StatusMsg);
}

///成交通知
void CtpTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
  CThostFtdcTradeField* trade = new CThostFtdcTradeField();
  memcpy(trade,  pTrade, sizeof(CThostFtdcTradeField));
  bool founded=false;
  string instrumentID = trade->InstrumentID;
  if(tradeMapVector.find(instrumentID) != tradeMapVector.end()){
    //说明此前有相应的合约编码的合约已经成交
    for(unsigned int i=0;i<tradeMapVector[instrumentID].size();i++){
      if(tradeMapVector[instrumentID][i]->BrokerOrderSeq == trade->BrokerOrderSeq){
        CThostFtdcTradeField* tmp = tradeMapVector[instrumentID][i];
        tradeMapVector[instrumentID][i] = trade;
        delete tmp;
        founded = true;
        break;
      }
    }
    if(!founded){
      tradeMapVector[instrumentID].push_back(trade);
    }
  }
  else{
    //第一次发现此成交合约编码类型的合约
    tradeMapVector[instrumentID].push_back(trade);
  }
  cerr<<" 回报 | 报单已成交...成交编号:"<<trade->TradeID<<endl;
}

void CtpTraderSpi::OnFrontDisconnected(int nReason)
{
	cerr<<" 响应 | 连接中断..."
	  << " reason=" << nReason << endl;
}


void CtpTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr<<" 响应 | 心跳超时警告..."
	  << " TimerLapse = " << nTimeLapse << endl;
}

void CtpTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	IsErrorRspInfo(pRspInfo);
}

bool CtpTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (ret){
    basicPrint(pRspInfo->ErrorMsg);
    cerr<<" 响应 错误信息 | "<<ConvertGb18030ToUtf8( pRspInfo->ErrorMsg)<<endl;
  }
	return ret;
}

void CtpTraderSpi::PrintOrders(){
  CThostFtdcOrderField* pOrder;
  unordered_map<string,vector<CThostFtdcOrderField*>>::iterator iter;
  for(iter = orderMapVector.begin();iter!=orderMapVector.end();iter++){
    vector<CThostFtdcOrderField*> orderList = iter->second;
    for(unsigned int i=0; i<orderList.size(); i++){
      pOrder = orderList[i];
     cerr<<" 报单 | 合约:"<<pOrder->InstrumentID
      <<" 方向:"<<MapDirection(pOrder->Direction,false)
      <<" 开平:"<<MapOffset(pOrder->CombOffsetFlag[0],false)
      <<" 价格:"<<pOrder->LimitPrice
      <<" 数量:"<<pOrder->VolumeTotalOriginal
      <<" 序号:"<<pOrder->BrokerOrderSeq
      <<" 报单编号:"<<pOrder->OrderSysID
      <<" 状态:"<<ConvertGb18030ToUtf8(pOrder->StatusMsg)<<endl;
      //<<" 状态:"<<pOrder->OrderStatus<<endl;
     // ConvertGb18030ToUtf8(pOrder->StatusMsg);
  }
  }
}
void CtpTraderSpi::PrintTrades(){
  CThostFtdcTradeField* pTrade;
  unordered_map<string,vector<CThostFtdcTradeField*>>::iterator iter;
  for(iter = tradeMapVector.begin();iter!=tradeMapVector.end();iter++){
   vector<CThostFtdcTradeField*> tradeList = iter->second;
       for(unsigned int i=0; i<tradeList.size(); i++){
    pTrade = tradeList[i];
    cerr<<" 成交 | 合约:"<< pTrade->InstrumentID
      <<" 方向:"<<MapDirection(pTrade->Direction,false)
      <<" 开平:"<<MapOffset(pTrade->OffsetFlag,false)
      <<" 价格:"<<pTrade->Price
      <<" 数量:"<<pTrade->Volume
      <<" 报单编号:"<<pTrade->OrderSysID
      <<" 成交编号:"<<pTrade->TradeID<<endl;
  }
  }
}
char MapDirection(char src, bool toOrig=true){
  if(toOrig){
    if('b'==src||'B'==src){src='0';}else if('s'==src||'S'==src){src='1';}
  }else{
    if('0'==src){src='B';}else if('1'==src){src='S';}
  }
  return src;
}
char MapOffset(char src, bool toOrig=true){
  if(toOrig){
    if('o'==src||'O'==src){src='0';}
    else if('c'==src||'C'==src){src='1';}
    else if('j'==src||'J'==src){src='3';}
  }else{
    if('0'==src){src='O';}
    else if('1'==src){src='C';}
    else if('3'==src){src='J';}
  }
  return src;
}
