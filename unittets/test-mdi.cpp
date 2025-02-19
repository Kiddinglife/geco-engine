#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "geco-test.h"

#define reset_geco_packet_fixed() \
        geco_packet.pk_comm_hdr.checksum = 0;\
        geco_packet.pk_comm_hdr.dest_port = htons(sender_dest_port);  \
        geco_packet.pk_comm_hdr.src_port = htons(sender_src_port);  \
        geco_packet.pk_comm_hdr.verification_tag = 0

#define init() \
            int i;\
           const int srcaddr4_size = 3;const int srcaddr6_size = 2;\
           const char* srcaddr4_ipstrs[srcaddr4_size] =\
               { "192.168.1.121", "192.168.1.132", "192.168.34.2" };\
           const char* srcaddr6_ipstrs[srcaddr6_size] =\
               { "2001:0db8:0a0b:12f0:0000:0000:0000:0001", "2607:f0d0:1002:0051:0000:0000:0000:0004" };\
           const int destaddr4_size = 3;\
           const int destaddr6_size = 2;\
           const char* destaddr4_ipstrs[destaddr4_size] =\
               { "192.168.1.122", "192.168.1.131", "192.168.34.1" };\
           const char* destaddr6_ipstrs[destaddr6_size] =\
               { "2001:0db8:0a0b:12f0:0000:0000:0000:0002", "2607:f0d0:1002:0051:0000:0000:0000:0005" };\
           const int cannot_found_ips4_size = 3;const int cannot_found_ips6_size = 2;\
           const char* cannot_found_ips4[cannot_found_ips4_size] =\
               { "192.168.1.123", "192.168.1.101", "192.168.34.5" };\
           const char* cannot_found_ips6[cannot_found_ips6_size] =\
               { "2001:0db8:0a0b:12f0:0000:0000:0000:0020", "2607:f0d0:1002:0051:0000:0000:0000:0100" };\
           sockaddrunion src_addres[srcaddr4_size]; \
           sockaddrunion src_addres6[srcaddr6_size]; \
           sockaddrunion dest_addres[destaddr4_size];\
           sockaddrunion dest_addres6[destaddr6_size]; \
           sockaddrunion cannot_found_addres4[cannot_found_ips4_size]; \
           sockaddrunion cannot_found_addres6[cannot_found_ips6_size]; \
           init_addrlist(true, 0, cannot_found_ips4, cannot_found_ips4_size, cannot_found_addres4);\
           init_addrlist(false, 0, cannot_found_ips6, cannot_found_ips6_size, cannot_found_addres6);\
           init_addrlist(true, 0, srcaddr4_ipstrs, srcaddr4_size, src_addres);\
           init_addrlist(false, 0, srcaddr6_ipstrs, srcaddr6_size, src_addres6);\
           init_addrlist(true, 0, destaddr4_ipstrs, destaddr4_size, dest_addres);\
           init_addrlist(false, 0, destaddr6_ipstrs, destaddr6_size, dest_addres6);\
           sockaddrunion local_addres[destaddr4_size + destaddr6_size];\
           sockaddrunion remote_addres[srcaddr4_size + srcaddr6_size];\
           for (i = 0; i < destaddr4_size; i++)\
           {\
               local_addres[i] = dest_addres[i];\
               remote_addres[i] = src_addres[i];\
           }\
           for (int j = 0; j < destaddr6_size; j++)\
           {\
               local_addres[i + j] = dest_addres6[j];\
               remote_addres[i + j] = src_addres6[j];\
           }\
           const int all_cannot_found_size = cannot_found_ips4_size + cannot_found_ips6_size;\
           sockaddrunion all_cannot_found_addres[all_cannot_found_size];\
           for (i = 0; i < cannot_found_ips4_size; i++)\
           {\
               all_cannot_found_addres[i] = cannot_found_addres4[i];\
           }\
           for (int j = 0; j < cannot_found_ips6_size; j++)\
           {\
               all_cannot_found_addres[i + j] = cannot_found_addres6[j];\
           }\
           ushort sender_src_port = 123;\
           ushort sender_dest_port = 456;\
           geco_packet_t geco_packet;\
           geco_packet_t* dctp_packet = &geco_packet;\
           geco_packet.pk_comm_hdr.checksum = 0;\
           geco_packet.pk_comm_hdr.dest_port = htons(sender_dest_port);  \
           geco_packet.pk_comm_hdr.src_port = htons(sender_src_port);  \
           geco_packet.pk_comm_hdr.verification_tag = 0;\
           geco_channel_t channel;\
           geco_instance_t inst;\
           channels_.push_back(&channel);\
           geco_instances_.push_back(&inst);\
           int rwnd = 512;\
           inst.supportedAddressTypes = SUPPORT_ADDRESS_TYPE_IPV4 | SUPPORT_ADDRESS_TYPE_IPV6;\
           inst.is_inaddr_any = false;\
           inst.is_in6addr_any = false;\
           inst.noOfInStreams = 6;\
           inst.noOfOutStreams = 6;\
           inst.local_port = sender_dest_port;\
           inst.local_addres_list = local_addres;\
           inst.local_addres_size = destaddr4_size + destaddr6_size;\
           channel.channel_id = 123;\
           channel.remote_addres = remote_addres;\
           channel.remote_addres_size = srcaddr4_size + srcaddr6_size;\
           channel.local_addres = local_addres;\
           channel.local_addres_size = destaddr4_size + destaddr6_size;\
           channel.remote_port = sender_src_port;\
           channel.local_port = sender_dest_port;\
           channel.remote_tag = 123;\
           channel.local_tag = 456;\
           channel.deleted = false;\
           channel.geco_inst = &inst;\
           smctrl_t smt;\
           channel.state_machine_control = &smt;\
           bundle_controller_t bctrl;\
           channel.bundle_control = &bctrl;\
           sockaddrunion* last_src_addr;\
           sockaddrunion* last_dest_addr;\
           uint written = 0;\
           uint dctp_packet_len = 0; int ret = good

static void
init_inst (geco_instance_t& inst, ushort destport, const char** src_ips,
           uint src_ips_len, sockaddrunion* dest)
{
  for (uint i = 0; i < src_ips_len; i++)
  {
    str2saddr (&dest[i], src_ips[i], destport);
  }
  inst.local_addres_size = src_ips_len;
  inst.local_addres_list = dest;
  inst.local_port = destport;
  geco_instances_.push_back (&inst);
}
static void
init_channel (geco_channel_t& channel, ushort srcport, ushort destport,
              const char** src_ips, uint src_ips_len, const char** dest_ips,
              uint dest_ips_len, sockaddrunion* srclist,
              sockaddrunion* destlist)
{
  for (uint i = 0; i < src_ips_len; i++)
  {
    str2saddr (&srclist[i], src_ips[i], srcport);
  }
  for (uint i = 0; i < dest_ips_len; i++)
  {
    str2saddr (&destlist[i], dest_ips[i], destport);
  }
  channel.remote_addres = srclist;
  channel.local_addres = destlist;
  channel.remote_port = srcport;
  channel.local_port = destport;
  channel.remote_addres_size = src_ips_len;
  channel.local_addres_size = dest_ips_len;
  channel.deleted = false;
  channel.is_IN6ADDR_ANY = false;
  channel.is_INADDR_ANY = false;
}
static void
init_addrlist (bool isip4, ushort port, const char** ipstrs, uint len,
               sockaddrunion* addrlist)
{
  for (uint i = 0; i < len; i++)
  {
    str2saddr (&addrlist[i], ipstrs[i], port);
  }
}

// passed on 28/02/2017
TEST(DISPATCHER_MODULE, test_find_geco_instance)
{
  // given
  const int destaddrsize = 6;
  const char* destipstrs[destaddrsize] =
    { "192.168.1.0", "192.168.1.1", "192.168.1.2", "192.168.1.3", "192.168.1.4",
        "192.168.1.5" };
  const ushort destport = 9989;

  geco_instance_t inst;
  sockaddrunion dest_addrs[destaddrsize];
  init_inst (inst, destport, destipstrs, destaddrsize, dest_addrs);

  sockaddrunion* last_dest_addr;
  ushort last_dest_port;
  geco_instance_t* ret = 0;

  // 1)  when is_in6addr_any and is_inaddr_any are both false
  inst.is_in6addr_any = inst.is_inaddr_any = false;

  // 1.1) and when last_dest_port, last_dest_addr and addr family both matches
  last_dest_port = inst.local_port;
  for (uint i = 0; i < inst.local_addres_size; i++)
  {
    last_dest_addr = &inst.local_addres_list[i];
    ret = mdi_find_geco_instance (last_dest_addr, last_dest_port);
    //  1.1.1) then should found this inst
    ASSERT_EQ(ret, &inst);
  }

  // 1.2) and when last_dest_port NOT mathces
  last_dest_port = inst.local_port - 1;
  for (uint i = 0; i < inst.local_addres_size; i++)
  {
    last_dest_addr = &inst.local_addres_list[i];
    ret = mdi_find_geco_instance (last_dest_addr, last_dest_port);
    //  1.2.1) then should NOT found this inst
    ASSERT_EQ(ret, nullptr);
  }

  // 1.3) and when last_dest_addr NOT matches
  last_dest_port = inst.local_port;
  for (uint i = 0; i < inst.local_addres_size; i++)
  {
    sockaddrunion tmp = inst.local_addres_list[i];
    s4addr(&tmp) -= 1;  // just minus to make it different
    last_dest_addr = &tmp;
    ret = mdi_find_geco_instance (last_dest_addr, last_dest_port);
    //  1.3.1) then should NOT found this inst
    ASSERT_EQ(ret, nullptr);
  }

  // 1.4) and when addr family NOT matches
  last_dest_port = inst.local_port;
  for (uint i = 0; i < inst.local_addres_size; i++)
  {
    sockaddrunion tmp = inst.local_addres_list[i];
    saddr_family(&tmp) == AF_INET ?
    saddr_family(&tmp) = AF_INET6 :
                                    saddr_family(&tmp) = AF_INET;
    last_dest_addr = &tmp;
    ret = mdi_find_geco_instance (last_dest_addr, last_dest_port);
    //  1.4.1) then should NOT found this inst
    ASSERT_EQ(ret, nullptr);
  }

  // 2) when  is_in6addr_any true, is_in4addr_any false
  // assume receive this packet and so addr family must  match
  // HERE local_addres_size IS ALL IP4 ADDR
  inst.is_in6addr_any = true;
  for (uint i = 0; i < inst.local_addres_size; i++)
  {
    sockaddrunion tmp = inst.local_addres_list[i];
    //  2.1) and when af(make it ip6) NOT matches (original ip4)
    saddr_family(&tmp) = AF_INET6;
    last_dest_addr = &tmp;
    ret = mdi_find_geco_instance (last_dest_addr, last_dest_port);
    //  2.1.1) should still found this inst
    ASSERT_EQ(ret, &inst);
    //  2.2) and when af matches (ip4)
    saddr_family(&tmp) = AF_INET;
    last_dest_addr = &tmp;
    ret = mdi_find_geco_instance (last_dest_addr, last_dest_port);
    //  2.2.1) should still found this inst
    ASSERT_EQ(ret, &inst);
    //  2.2.1) and when dest addr NOT matches
    s4addr(&tmp) -= 1; // just minus to make it not to be found in local_addres_
    saddr_family(&tmp) = AF_INET;
    last_dest_addr = &tmp;
    ret = mdi_find_geco_instance (last_dest_addr, last_dest_port);
    //  2.2.1) should NOT found this inst
    ASSERT_EQ(ret, nullptr);
  }
}

// passed on 28/02/2017
TEST(DISPATCHER_MODULE, test_find_channel)
{
  // given
  const int src_ips_len = 3;
  const char* src_ips[src_ips_len] =
    { "192.168.1.0", "192.168.1.1", "192.168.1.2" };
  const int dest_ips_len = 3;
  const char* dest_ips[dest_ips_len] =
    { "192.168.1.3", "192.168.1.4", "192.168.1.5" };
  const ushort ports[] =
    { 100, 101 };  // src-dest
  sockaddrunion remote_addres[src_ips_len];
  sockaddrunion local_addres[dest_ips_len];
  geco_channel_t channel;
  for (uint i = 0; i < src_ips_len; i++)
  {
    str2saddr (&remote_addres[i], src_ips[i], ports[0]);
  }
  for (uint i = 0; i < dest_ips_len; i++)
  {
    str2saddr (&local_addres[i], dest_ips[i], ports[1]);
  }
  channel.channel_id = 5;
  channel.remote_addres = 0;
  channel.local_addres = local_addres;
  channel.remote_port = ports[0];
  channel.local_port = ports[1];
  channel.remote_addres_size = 0;
  channel.local_addres_size = dest_ips_len;
  channel.deleted = false;
  channel.is_IN6ADDR_ANY = false;
  channel.is_INADDR_ANY = false;

  // stub the related variables
  geco_channel_t* chanids[8] =
    { 0 };
  channels_ = chanids;
  curr_channel_ = &channel;
  channels_[5] = curr_channel_;
  mdi_set_channel_remoteaddrlist (remote_addres, src_ips_len);
  print_addrlist (channel.local_addres, channel.local_addres_size);
  print_addrlist (channel.remote_addres, channel.remote_addres_size);

  //temps
  geco_channel_t* found;
  sockaddrunion* last_src_addr;
  sockaddrunion* last_dest_addr;
  ushort last_src_port = channel.remote_port;
  ushort last_dest_port = channel.local_port;
  sockaddrunion tmp_addr;

  // 0) when all matching
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    last_dest_addr = &channel.local_addres[i];
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      last_src_addr = &channel.remote_addres[j];
      //then should find channel
      curr_trans_addr_.local_saddr = last_dest_addr;
      curr_trans_addr_.peer_saddr = last_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, curr_channel_);
    }
  }

  // 1) when only src port not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    last_dest_addr = &channel.local_addres[i];
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      tmp_addr = channel.remote_addres[j];
      tmp_addr.sin.sin_port -= 1; //just make it not equal to the one stored in channel
      // then should not find channel
      curr_trans_addr_.local_saddr = last_dest_addr;
      curr_trans_addr_.peer_saddr = &tmp_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }

  //2) when dest port not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    tmp_addr = channel.local_addres[i];
    tmp_addr.sin.sin_port -= 1; //just make it not equal to the one stored in channel
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      last_src_addr = &channel.remote_addres[j];
      //then should not find channel
      curr_trans_addr_.local_saddr = &tmp_addr;
      curr_trans_addr_.peer_saddr = last_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }

  //3) when dest and src port not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    sockaddrunion tmp_local_addr = channel.local_addres[i];
    tmp_local_addr.sin.sin_port -= 1; //just make it not equal to the one stored in channel
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      sockaddrunion tmp_src_addr = channel.remote_addres[j];
      tmp_local_addr.sin.sin_port -= 1; //just make it not equal to the one stored in channel
      //then should not find channel
      curr_trans_addr_.local_saddr = &tmp_local_addr;
      curr_trans_addr_.peer_saddr = &tmp_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }

  //4) when dest addr not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    tmp_addr = channel.local_addres[i];
    s4addr(&tmp_addr) -= 1;  // just minus to make it different
    last_dest_addr = &tmp_addr;
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      last_src_addr = &channel.remote_addres[j];
      //then should not find channel
      curr_trans_addr_.local_saddr = last_dest_addr;
      curr_trans_addr_.peer_saddr = last_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }

  //5) when src addr not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    last_dest_addr = &channel.local_addres[i];
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      tmp_addr = channel.remote_addres[i];
      s4addr(&tmp_addr) -= 1;  // just minus to make it different
      last_src_addr = &tmp_addr;
      //then should not find channel
      curr_trans_addr_.local_saddr = last_dest_addr;
      curr_trans_addr_.peer_saddr = last_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }

  //6) when  dest and addr not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    tmp_addr = channel.local_addres[i];
    s4addr(&tmp_addr) -= 1;  // just minus to make it different
    last_dest_addr = &tmp_addr;
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      sockaddrunion tmp_addr2;
      tmp_addr2 = channel.remote_addres[i];
      s4addr(&tmp_addr2) -= 1;  // just minus to make it different
      last_src_addr = &tmp_addr2;
      //then should not find channel
      curr_trans_addr_.local_saddr = last_dest_addr;
      curr_trans_addr_.peer_saddr = last_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }

  //7) when dest addr family not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    tmp_addr = channel.local_addres[i];
    saddr_family(&tmp_addr) == AF_INET ?
    saddr_family(&tmp_addr) = AF_INET6 :
                                         saddr_family(&tmp_addr) = AF_INET;
    last_dest_addr = &tmp_addr;
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      last_src_addr = &channel.remote_addres[j];
      //then should not find channel
      curr_trans_addr_.local_saddr = last_dest_addr;
      curr_trans_addr_.peer_saddr = last_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }
  //8) when src addr family not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    last_dest_addr = &channel.local_addres[i];
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      tmp_addr = channel.remote_addres[i];
      saddr_family(&tmp_addr) == AF_INET ?
      saddr_family(&tmp_addr) = AF_INET6 :
                                           saddr_family(&tmp_addr) =
                                           AF_INET;
      last_src_addr = &tmp_addr;
      //then should not find channel
      curr_trans_addr_.local_saddr = last_dest_addr;
      curr_trans_addr_.peer_saddr = last_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }

  //9) when dest and src af both not equal
  for (uint i = 0; i < channel.local_addres_size; i++)
  {
    tmp_addr = channel.local_addres[i];
    saddr_family(&tmp_addr) == AF_INET ?
    saddr_family(&tmp_addr) = AF_INET6 :
                                         saddr_family(&tmp_addr) = AF_INET;
    last_dest_addr = &tmp_addr;
    for (uint j = 0; j < channel.remote_addres_size; j++)
    {
      sockaddrunion tmp_addr2;
      tmp_addr2 = channel.remote_addres[i];
      saddr_family(&tmp_addr2) == AF_INET ?
      saddr_family(&tmp_addr2) = AF_INET6 :
                                            saddr_family(&tmp_addr2) =
                                            AF_INET;
      last_src_addr = &tmp_addr2;
      //then should not find channel
      curr_trans_addr_.local_saddr = last_dest_addr;
      curr_trans_addr_.peer_saddr = last_src_addr;
      found = mdi_find_channel ();
      ASSERT_EQ(found, nullptr);
    }
  }
}

// last run and passed on 22 Agu 2016
//TEST(DISPATCHER_MODULE, test_validate_dest_addr)
//{
//	/*8)
//	 * now we can validate if dest_addr in localaddress
//	 * this method internally uses curr_geco_instance_ and curr_channel_
//	 * so we must call it right here
//	 */
//	int i;
//	const char* addres[6] =
//	{ "192.168.1.0", "192.168.1.1", "192.168.1.2", "192.168.1.3", "192.168.1.4",
//		"192.168.1.5" };
//	const int addres_cnt = 6;
//	const ushort ports[addres_cnt] =
//	{ 100, 101 };  // src-dest
//	sockaddrunion remote_addres[addres_cnt / 2];
//	sockaddrunion local_addres[addres_cnt / 2];
//
//	for (i = 0; i < addres_cnt; i++)
//	{
//		if (i < addres_cnt / 2)
//		{
//			str2saddr(&remote_addres[i], addres[i], ports[0]);
//		}
//		else
//		{
//			int idx = i % (addres_cnt / 2);
//			str2saddr(&local_addres[idx], addres[i], ports[1]);
//		}
//	}
//
//	geco_channel_t channel;
//	channel.remote_addres = remote_addres;
//	channel.local_addres = local_addres;
//	channel.remote_port = ports[0];
//	channel.local_port = ports[1];
//	channel.remote_addres_size = addres_cnt / 2;
//	channel.local_addres_size = addres_cnt / 2;
//	channel.deleted = false;
//
//	geco_instance_t inst;
//	inst.local_addres_size = addres_cnt / 2;
//	inst.local_addres_list = local_addres;
//	inst.local_port = ports[1];
//	channel.geco_inst = &inst;
//
//	channels_.push_back(&channel);
//	geco_instances_.push_back(&inst);
//	sockaddrunion* last_dest_addr;
//	bool ret;
//
//	//1) test return true when both of channel and inst are NULL
//	curr_channel_ = NULL;
//	curr_geco_instance_ = NULL;
//	last_dest_addr = local_addres + 2;
//	ret = validate_dest_addr(last_dest_addr);
//	ASSERT_EQ(ret, true);
//
//	//2) test return true when curr_channel_ NOT NULL
//	curr_channel_ = &channel;
//	curr_geco_instance_ = &inst;
//	last_dest_addr = local_addres + 2;
//	ret = validate_dest_addr(last_dest_addr);
//	ASSERT_EQ(ret, true);
//
//	//2) test return true when curr_channel_  NULL,
//	// inst NOT NULL, is_inaddr_any false, is_in6addr_any false;
//	curr_channel_ = NULL;
//	curr_geco_instance_ = &inst;
//	inst.is_inaddr_any = false;
//	inst.is_in6addr_any = false;
//	last_dest_addr = local_addres + 2;
//	ret = validate_dest_addr(last_dest_addr);
//	ASSERT_EQ(ret, true);
//
//	//3) test return true when curr_channel_  NULL inst NOT NULL, is_inaddr_any true, is_in6addr_any false;
//	curr_channel_ = NULL;
//	curr_geco_instance_ = &inst;
//	inst.is_inaddr_any = true;
//	inst.is_in6addr_any = false;
//	last_dest_addr = local_addres + 2;
//	ret = validate_dest_addr(last_dest_addr);
//	ASSERT_EQ(ret, true);
//
//	//3) test return true when curr_channel_  NULL inst NOT NULL, is_inaddr_any false, is_in6addr_any true;
//	curr_channel_ = NULL;
//	curr_geco_instance_ = &inst;
//	inst.is_inaddr_any = false;
//	inst.is_in6addr_any = true;
//	last_dest_addr = local_addres + 2;
//	ret = validate_dest_addr(last_dest_addr);
//	ASSERT_EQ(ret, false);
//
//	//3) test return false when curr_channel_  NULL inst NOT NULL, is_inaddr_any false, is_in6addr_any false;
//	curr_channel_ = NULL;
//	curr_geco_instance_ = &inst;
//	inst.is_inaddr_any = false;
//	inst.is_in6addr_any = false;
//	last_dest_addr = remote_addres + 2; // we use remote addr as local addr that will not be found
//	ret = validate_dest_addr(last_dest_addr);
//	ASSERT_EQ(ret, true);
//
//	//3) test return false when curr_channel_  NOT NULL,  is_inaddr_any false, is_in6addr_any false;
//	curr_channel_ = &channel;
//	curr_geco_instance_ = &inst;
//	inst.is_inaddr_any = false;
//	inst.is_in6addr_any = false;
//	last_dest_addr = remote_addres + 2; // we use remote addr as local addr that will not be found
//	ret = validate_dest_addr(last_dest_addr);
//	//should return true
//	ASSERT_EQ(ret, true);
//
//}

// passed on 28/02/2017
TEST(DISPATCHER_MODULE, test_find_chunk_types)
{
  geco_packet_t geco_packet;
  geco_packet.pk_comm_hdr.checksum = 0;
  geco_packet.pk_comm_hdr.dest_port = htons (
      (generate_random_uint32 () % USHRT_MAX));
  geco_packet.pk_comm_hdr.src_port = htons (
      (generate_random_uint32 () % USHRT_MAX));
  geco_packet.pk_comm_hdr.verification_tag = htons (
      (generate_random_uint32 ()));

  // given one data chunk
  uint offset = 0;
  uint chunklen = 0;
  uchar* wt = geco_packet.chunk;
  uint datalen = 101;
  chunklen = DCHUNK_R_O_S_FIXED_SIZES + datalen;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_DATA;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 116);
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_DATA);
  wt += chunklen;

  //given one sack chunk
  datalen = 31;
  chunklen = datalen + SACK_CHUNK_FIXED_SIZE + CHUNK_FIXED_SIZE;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_SACK;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  //116+4+12+31 = 132+31 = 163
  while (chunklen % 4)
    chunklen++;
  ASSERT_EQ(((chunk_fixed_t* )(geco_packet.chunk + offset))->chunk_id,
            CHUNK_SACK);
  offset += chunklen;
  ASSERT_EQ(offset, 164);
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_SACK);
  wt += chunklen;

  //given one init chunk
  datalen = 21;
  chunklen = datalen + INIT_CHUNK_FIXED_SIZES;  //21+20=41
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_INIT;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  ASSERT_EQ(geco_packet.chunk + offset, wt);
  ASSERT_EQ(((chunk_fixed_t* )(geco_packet.chunk + offset))->chunk_id,
            CHUNK_INIT);
  offset += chunklen;
  ASSERT_EQ(offset, 208);  // 164+4+16+21= 205
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_INIT);
  wt += chunklen;

  //given one init ack chunk
  datalen = 21;
  chunklen = datalen + INIT_CHUNK_FIXED_SIZES;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_INIT_ACK;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  ASSERT_EQ(((chunk_fixed_t* )(geco_packet.chunk + offset))->chunk_id,
            CHUNK_INIT_ACK);
  offset += chunklen;
  ASSERT_EQ(offset, 252);  // 208+20+21 = 228+21=249
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_INIT_ACK);
  wt += chunklen;

  //given CHUNK_SHUTDOWN
  chunklen = 4 + CHUNK_FIXED_SIZE;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_SHUTDOWN;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 260);  // 252+8 = 260
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_SHUTDOWN);
  wt += chunklen;

  //given CHUNK_SHUTDOWN_ACK
  chunklen = CHUNK_FIXED_SIZE;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_SHUTDOWN_ACK;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 264);  // 260+4 = 264
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_SHUTDOWN_ACK);
  wt += chunklen;

  //1) when  given good chunks
  uint total_chunks_count;
  uint chunk_types = find_chunk_types (geco_packet.chunk, offset,
                                       &total_chunks_count);
  //then should find all chunks
  ASSERT_EQ(contains_chunk(CHUNK_DATA, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_INIT, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_INIT_ACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_ACK, chunk_types), 2);
  ASSERT_EQ(total_chunks_count, 6);

  //2) when there are bad chunks whose chun len < CHUNK_FIXED_SIZE
  //CHUNK_SHUTDOWN_COMPLETE
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_SHUTDOWN_COMPLETE;
  ((chunk_fixed_t*) wt)->chunk_length = htons (3);
  offset += 4;
  ASSERT_EQ(offset, 268);  // 264+4 = 268
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_SHUTDOWN_COMPLETE);
  wt += 4;
  chunk_types = find_chunk_types (geco_packet.chunk, offset,
                                  &total_chunks_count);
  //then should find all good chunks skipping bad one
  ASSERT_EQ(contains_chunk(CHUNK_DATA, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_INIT, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_INIT_ACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_ACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_COMPLETE, chunk_types), 0);
  ASSERT_EQ(total_chunks_count, 6);

  //3) when chunk_len + read_len > packet_val_len
  chunk_types = find_chunk_types (geco_packet.chunk, offset - 4,
                                  &total_chunks_count);
  //then should find all good chunks skipping bad one
  ASSERT_EQ(contains_chunk(CHUNK_DATA, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_INIT, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_INIT_ACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_ACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_COMPLETE, chunk_types), 0);
  ASSERT_EQ(total_chunks_count, 6);

  //4) when ther is CHUNK_SHUTDOWN_ACK
  chunk_types = find_chunk_types (wt - 8, offset - 8, &total_chunks_count);
  ASSERT_EQ(contains_chunk(CHUNK_DATA, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_SACK, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_INIT, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_INIT_ACK, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_ACK, chunk_types), 1);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_COMPLETE, chunk_types), 0);
  ASSERT_EQ(total_chunks_count, 1);

  //5) when there are two repeated shutdown ack chunk
  chunklen = CHUNK_FIXED_SIZE;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_SHUTDOWN_ACK;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 272);  // 260+4 = 264
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_SHUTDOWN_ACK);
  wt += chunklen;
  chunklen = CHUNK_FIXED_SIZE;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_SHUTDOWN_ACK;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 276);  // 260+4 = 264
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_SHUTDOWN_ACK);
  wt += chunklen;
  chunk_types = find_chunk_types (wt - 8, offset - 8, &total_chunks_count);
  //then contains_chunk() should find 1 shutdown ack chunk but total_chunks_count should be 2 and not find other chunk types
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_ACK, chunk_types), 1);
  ASSERT_EQ(total_chunks_count, 2);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN_COMPLETE, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_DATA, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_SACK, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_INIT, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_INIT_ACK, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_SHUTDOWN, chunk_types), 0);
}

// passed on 28/02/2017
TEST(DISPATCHER_MODULE, test_find_first_chunk_of)
{
  geco_packet_t geco_packet;
  geco_packet.pk_comm_hdr.checksum = 0;
  geco_packet.pk_comm_hdr.dest_port = htons (
      (generate_random_uint32 () % USHRT_MAX));
  geco_packet.pk_comm_hdr.src_port = htons (
      (generate_random_uint32 () % USHRT_MAX));
  geco_packet.pk_comm_hdr.verification_tag = htonl (
      (generate_random_uint32 ()));

  // given one data chunk
  uint offset = 0;
  uint chunklen = 0;
  uchar* wt = geco_packet.chunk;
  uint datalen = 101;
  chunklen = DCHUNK_R_O_S_FIXED_SIZES + datalen;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_DATA;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 116);
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_DATA);
  wt += chunklen;

  //given another data chunk
  datalen = 35;
  chunklen = DCHUNK_R_O_S_FIXED_SIZES + datalen;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_DATA;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 164);
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_DATA);
  wt += chunklen;

  //given one sack chunk
  datalen = 31;
  chunklen = datalen + SACK_CHUNK_FIXED_SIZE + CHUNK_FIXED_SIZE;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_SACK;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  //116+4+12+31 = 132+31 = 163
  while (chunklen % 4)
    chunklen++;
  ASSERT_EQ(((chunk_fixed_t* )(geco_packet.chunk + offset))->chunk_id,
            CHUNK_SACK);
  offset += chunklen;
  ASSERT_EQ(offset, 212);
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_SACK);
  wt += chunklen;

  // then should find them
  ASSERT_EQ(mch_find_first_chunk_of(geco_packet.chunk, offset, CHUNK_DATA),
            geco_packet.chunk);
  ASSERT_EQ(mch_find_first_chunk_of(geco_packet.chunk, offset, CHUNK_SACK),
            wt - chunklen);
  ASSERT_EQ(mch_find_first_chunk_of(geco_packet.chunk, offset, CHUNK_INIT),
            nullptr);
  ASSERT_EQ(mch_find_first_chunk_of(geco_packet.chunk, offset - 45, CHUNK_SACK),
            nullptr);

  //1) when chunk_len < CHUNK_FIXED_SIZE
  chunklen = 3;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_SHUTDOWN_ACK;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 216);
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_SHUTDOWN_ACK);
  wt += chunklen;
  //then should not find this chunk
  ASSERT_EQ(
      mch_find_first_chunk_of(geco_packet.chunk, offset, CHUNK_SHUTDOWN_ACK),
      nullptr);

  //2) when chunk_len + read_len > packet_val_len
  offset -= chunklen;
  wt -= chunklen;
  chunklen = 4;
  ((chunk_fixed_t*) wt)->chunk_id = CHUNK_INIT_ACK;
  ((chunk_fixed_t*) wt)->chunk_length = htons (chunklen + 1);
  while (chunklen % 4)
    chunklen++;
  offset += chunklen;
  ASSERT_EQ(offset, 216);
  ASSERT_EQ(((chunk_fixed_t* )wt)->chunk_id, CHUNK_INIT_ACK);
  wt += chunklen;
  //then should not find this chunk
  ASSERT_EQ(mch_find_first_chunk_of(geco_packet.chunk, offset, CHUNK_INIT_ACK),
            nullptr);
}

// last run and passed on 22 Agu 2016
//TEST(DISPATCHER_MODULE, test_read_peer_addreslist)
//{
//	ASSERT_EQ(sizeof(in_addr), 4);
//	ASSERT_EQ(sizeof(in6_addr), 16);
//	//////////////////////////////////////////////////////////////////////////////
//	geco_packet_t geco_packet;
//	geco_packet.pk_comm_hdr.checksum = 0;
//	geco_packet.pk_comm_hdr.dest_port = htons(
//		(generate_random_uint32() % USHRT_MAX));
//	geco_packet.pk_comm_hdr.src_port = htons(
//		(generate_random_uint32() % USHRT_MAX));
//	geco_packet.pk_comm_hdr.verification_tag = htonl(
//		(generate_random_uint32()));
//	//////////////////////////////////////////////////////////////////////////////
//	init_chunk_t* init_chunk = (init_chunk_t*)(geco_packet.chunk);
//	init_chunk->chunk_header.chunk_id = CHUNK_INIT;
//	init_chunk->chunk_header.chunk_flags = 0;
//	//////////////////////////////////////////////////////////////////////////////
//	int i;
//	const char* addres[] =
//	{ "192.168.1.121", "192.168.1.132", "192.168.34.2" };
//	const char* addres6[] =
//	{ "2001:0db8:0a0b:12f0:0000:0000:0000:0001",
//		"2607:f0d0:1002:0051:0000:0000:0000:0004" };
//	sockaddrunion local_addres[3];
//	sockaddrunion local_addres6[2];
//	init_addrlist(true, 0, addres, 3, local_addres);
//	init_addrlist(false, 0, addres6, 2, local_addres6);
//	//////////////////////////////////////////////////////////////////////////////
//	uint offset = 0;
//	offset += mch_write_vlp_supportedaddrtypes(init_chunk->variableParams, true,
//		false, false);
//	offset += mch_write_vlp_addrlist(init_chunk->variableParams + offset, local_addres,
//		3);
//	offset += mch_write_vlp_addrlist(init_chunk->variableParams + offset, local_addres6, 2);
//	//////////////////////////////////////////////////////////////////////////////
//	ASSERT_EQ(offset, 72);
//	init_chunk->chunk_header.chunk_length = htons(
//		INIT_CHUNK_FIXED_SIZES + offset);
//	//////////////////////////////////////////////////////////////////////////////
//	sockaddrunion peer_addreslist[MAX_NUM_ADDRESSES];
//	defaultlocaladdrlistsize_ = 0;
//	//////////////////////////////////////////////////////////////////////////////
//	char buf[MAX_IPADDR_STR_LEN];
//	ushort port;
//	//////////////////////////////////////////////////////////////////////////////
//	sockaddrunion last_source_addr;
//	last_source_addr_ = &last_source_addr;
//	int ret;
//	//////////////////////////////////////////////////////////////////////////////
//	uint peersupportedtypes = 0;
//	str2saddr(&last_source_addr, "2607:f0d0:1002:0051:0000:0000:0000:0005", 0);
//	ret = mdi_read_peer_addreslist(peer_addreslist, geco_packet.chunk,
//		offset + INIT_CHUNK_FIXED_SIZES,
//		SUPPORT_ADDRESS_TYPE_IPV4,
//		&peersupportedtypes, true, false);
//	ASSERT_EQ(ret, 3); //3 ip4 addrs  but last src addr ths is ip6 not supported by us
//	//ip4 addrs  plus last src addr is ip6
//	ASSERT_EQ(peersupportedtypes,
//		SUPPORT_ADDRESS_TYPE_IPV4 | SUPPORT_ADDRESS_TYPE_IPV6);
//	//////////////////////////////////////////////////////////////////////////////
//	for (i = 0; i < 3; ++i)
//	{
//		saddr2str(&peer_addreslist[i], buf, MAX_IPADDR_STR_LEN, &port);
//		EVENTLOG1(VERBOSE, "peer ip4addr: %s\n", buf);
//		saddr2str(&local_addres[i], buf, MAX_IPADDR_STR_LEN, &port);
//		EVENTLOG1(VERBOSE, "record ip4addr: %s\n", buf);
//		EXPECT_TRUE(saddr_equals(&peer_addreslist[i], &local_addres[i]));
//	}
//	//////////////////////////////////////////////////////////////////////////////
//	str2saddr(&last_source_addr, "192.168.5.123", 0);
//	ret = mdi_read_peer_addreslist(
//		peer_addreslist, geco_packet.chunk, offset + INIT_CHUNK_FIXED_SIZES,
//		SUPPORT_ADDRESS_TYPE_IPV4 | SUPPORT_ADDRESS_TYPE_IPV6, NULL, true, false);
//	ASSERT_EQ(ret, 6);
//	for (i = 0; i < 3; ++i)
//	{
//		EXPECT_TRUE(saddr_equals(&peer_addreslist[i], &local_addres[i], true));
//	}
//	for (i = 3; i < 5; ++i)
//	{
//		EXPECT_TRUE(
//			saddr_equals(&peer_addreslist[i], &local_addres6[i - 3], true));
//	}
//	EXPECT_TRUE(saddr_equals(&peer_addreslist[5], &last_source_addr, true));
//	str2saddr(&last_source_addr, "2607:f0d0:1002:0051:0000:0000:0000:0005", 0);
//	ret = mdi_read_peer_addreslist(peer_addreslist, geco_packet.chunk,
//		offset + INIT_CHUNK_FIXED_SIZES,
//		SUPPORT_ADDRESS_TYPE_IPV6, NULL, true, false);
//	ASSERT_EQ(ret, 3);  //2 + last_source_addr_ = 3
//	for (i = 0; i < 2; ++i)
//	{
//		saddr2str(&peer_addreslist[i], buf, MAX_IPADDR_STR_LEN, &port);
//		EVENTLOG1(VERBOSE, "peer ip6addr: %s\n", buf);
//		saddr2str(&local_addres6[i], buf, MAX_IPADDR_STR_LEN, &port);
//		EVENTLOG1(VERBOSE, "record ip6addr: %s\n", buf);
//		EXPECT_TRUE(saddr_equals(&peer_addreslist[i], &local_addres6[i], true));
//	}
//	EXPECT_TRUE(saddr_equals(&peer_addreslist[2], &last_source_addr, true));
//}

// passed on 28/02/2017
TEST(DISPATCHER_MODULE, test_contain_local_addr)
{
  /**
   * check if local addr is found
   * eg. ip4 loopback 127.0.0.1 or ip4  ethernet local addr 192.168.1.107 or public ip4 addr
   * containslocaladdr(sockaddrunion* addr_list,uint addr_list_num);
   */
  int i;
  const char* addres[] =
    { "192.168.1.121", "192.168.1.132", "192.168.34.2" };
  const char* addres6[] =
    { "2001:0db8:0a0b:12f0:0000:0000:0000:0001",
        "2607:f0d0:1002:0051:0000:0000:0000:0004" };
  sockaddrunion local_addres[3];
  sockaddrunion local_addres6[2];
  for (i = 0; i < 3; i++)
  {
    str2saddr (&local_addres[i], addres[i], 0);
  }
  for (i = 0; i < 2; i++)
  {
    str2saddr (&local_addres6[i], addres6[i], 0);
  }
  geco_instance_t inst;
  inst.supportedAddressTypes = SUPPORT_ADDRESS_TYPE_IPV4;
  inst.local_addres_size = 3;
  inst.local_addres_list = local_addres;
  //////////////////////////////////////////////////////////////////////////////
  geco_instances_.push_back (&inst);
  sockaddrunion tmpaddr;
  //////////////////////////////////////////////////////////////////////////////
  //1) test branch 1 curr geco_inst and curr channel both NULL
  //1.1) test no local addr presents
  EXPECT_FALSE(mdi_contains_localaddr (local_addres, 3));
  EXPECT_FALSE(mdi_contains_localaddr (local_addres6, 2));
  //1.2) test  local addr presents
  tmpaddr = local_addres[1];
  str2saddr (&local_addres[1], "127.0.0.1", 0);
  EXPECT_TRUE(mdi_contains_localaddr (local_addres, 3));
  local_addres[1] = tmpaddr;
  tmpaddr = local_addres6[1];
  str2saddr (&local_addres6[1], "::1", 0);
  EXPECT_TRUE(mdi_contains_localaddr (local_addres6, 2));
  local_addres6[1] = tmpaddr;
  //////////////////////////////////////////////////////////////////////////////
  //2) test branch 2 curr_geco_instance_ NOT NULL
  curr_geco_instance_ = &inst;
  //2.1) test local addr in curr gecio inst local addres list
  tmpaddr = local_addres[1];
  EXPECT_TRUE(mdi_contains_localaddr (&tmpaddr, 1));
  //2.1) test no local addr in curr gecio inst local addres list
  str2saddr (&tmpaddr, "221.123.45.12", 0);
  EXPECT_FALSE(mdi_contains_localaddr (&tmpaddr, 1));
}

// passed on 28/02/2017
TEST(DISPATCHER_MODULE, test_find_vlparam_from_setup_chunk)
{
  geco_packet_t geco_packet;
  geco_packet.pk_comm_hdr.checksum = 0;
  geco_packet.pk_comm_hdr.dest_port = htons (
      (generate_random_uint32 () % USHRT_MAX));
  geco_packet.pk_comm_hdr.src_port = htons (
      (generate_random_uint32 () % USHRT_MAX));
  geco_packet.pk_comm_hdr.verification_tag = htons (
      (generate_random_uint32 ()));
  //////////////////////////////////////////////////////////////////////////////
  init_chunk_t* init_chunk = (init_chunk_t*) (geco_packet.chunk);
  init_chunk->chunk_header.chunk_id = CHUNK_INIT;
  init_chunk->chunk_header.chunk_flags = 0;
  //////////////////////////////////////////////////////////////////////////////
  const char* hn = "www.baidu.com";
  ((vlparam_fixed_t*) init_chunk->variableParams)->param_type = htons (
  VLPARAM_HOST_NAME_ADDR);
  ((vlparam_fixed_t*) init_chunk->variableParams)->param_length = htons (
      4 + strlen (hn));
  strcpy ((char*) (init_chunk->variableParams + 4), hn);
  //////////////////////////////////////////////////////////////////////////////
  uint len = 4 + strlen (hn) + INIT_CHUNK_FIXED_SIZES;
  init_chunk->chunk_header.chunk_length = htons (len);
  while (len % 4)
    ++len;
  uchar* ret = mch_read_vlparam_init_chunk (geco_packet.chunk, len,
  VLPARAM_HOST_NAME_ADDR);
  ASSERT_EQ(ret, init_chunk->variableParams);
  //////////////////////////////////////////////////////////////////////////////
  ret = mch_read_vlparam_init_chunk (geco_packet.chunk, len,
  VLPARAM_COOKIE);
  ASSERT_EQ(ret, nullptr);
  ret = mch_read_vlparam_init_chunk (geco_packet.chunk, len,
  VLPARAM_SUPPORTED_ADDR_TYPES);
  ASSERT_EQ(ret, nullptr);

}

// last run and passed on 29 Agu 2016
//TEST(DISPATCHER_MODULE, test_bundle_ctrl_chunk)
//{
//	sockaddrunion last_drc_addr;
//	str2saddr(&last_drc_addr, "127.0.0.1", 456);
//	last_source_addr_ = &last_drc_addr;
//	last_dest_port_ = 123;
//	last_src_port_ = 456;
//	last_init_tag_ = 12345;
//	//////////////////////////////////////////////////////////////////////////////
//	uint cid = mch_make_simple_chunk(CHUNK_SHUTDOWN_COMPLETE,
//		FLAG_TBIT_SET);
//	ASSERT_EQ(simple_chunks_[cid]->chunk_header.chunk_flags, 0x01);
//	curr_write_pos_[cid] += 24;
//	simple_chunk_t* simple_chunk_t_ptr_ = mch_complete_simple_chunk(cid);
//	ASSERT_EQ(simple_chunks_[cid]->chunk_header.chunk_length, htons(28));
//	ASSERT_EQ(completed_chunks_[cid], true);
//	mch_free_simple_chunk(cid);
//	default_bundle_ctrl_.reset();
//
//	cid = mch_make_simple_chunk(CHUNK_SHUTDOWN_COMPLETE,
//		FLAG_TBIT_UNSET);
//	ASSERT_EQ(simple_chunks_[cid]->chunk_header.chunk_flags, 0);
//	simple_chunk_t_ptr_ = mch_complete_simple_chunk(cid);
//	ASSERT_EQ(simple_chunks_[cid]->chunk_header.chunk_length, htons(4));
//	ASSERT_EQ(completed_chunks_[cid], true);
//	mch_free_simple_chunk(cid);
//	default_bundle_ctrl_.reset();
//	//////////////////////////////////////////////////////////////////////////////
//	//1) if packet length < max_geco_
//	cid = mch_make_simple_chunk(CHUNK_SHUTDOWN_COMPLETE,
//		FLAG_TBIT_UNSET);
//	simple_chunk_t_ptr_ = mch_complete_simple_chunk(cid);
//	//  1.1) if dest_index == NULL
//	mdi_bundle_ctrl_chunk(simple_chunk_t_ptr_, NULL);
//	//      1.1.1) got_send_address shoul be false && requested_destination should be zero
//	EXPECT_FALSE(default_bundle_ctrl_.got_send_address);
//	ASSERT_EQ(default_bundle_ctrl_.requested_destination, 0);
//	mch_free_simple_chunk(cid);
//	default_bundle_ctrl_.reset();
//	//  1.2)if dest_index != NULL
//	int path = 6;
//	cid = mch_make_simple_chunk(CHUNK_SHUTDOWN_COMPLETE,
//		FLAG_TBIT_UNSET);
//	simple_chunk_t_ptr_ = mch_complete_simple_chunk(cid);
//	mdi_bundle_ctrl_chunk(simple_chunk_t_ptr_, &path);
//	//      1.2.1) got_send_address shoul be true && requested_destination should be 6
//	EXPECT_TRUE(default_bundle_ctrl_.got_send_address);
//	ASSERT_EQ(default_bundle_ctrl_.requested_destination, path);
//	mch_free_simple_chunk(cid);
//	default_bundle_ctrl_.reset();
//	//////////////////////////////////////////////////////////////////////////////
//	//2) if packet length == max_geco_packet_length
//	cid = mch_make_simple_chunk(CHUNK_SHUTDOWN_COMPLETE,
//		FLAG_TBIT_SET);
//	curr_write_pos_[cid] += MAX_NETWORK_PACKET_VALUE_SIZE - 4;
//	simple_chunk_t_ptr_ = mch_complete_simple_chunk(cid);
//	ASSERT_EQ(ntohs(simple_chunk_t_ptr_->chunk_header.chunk_length),
//		MAX_NETWORK_PACKET_VALUE_SIZE);
//	ASSERT_EQ(get_bundle_total_size(&default_bundle_ctrl_),
//		UDP_GECO_PACKET_FIXED_SIZES);
//	//  2.1 should not force send
//	mdi_bundle_ctrl_chunk(simple_chunk_t_ptr_, &path);
//	ASSERT_EQ(get_bundle_total_size(&default_bundle_ctrl_),
//		MAX_GECO_PACKET_SIZE);
//	mch_free_simple_chunk(cid);
//	//3) if packet length > max_geco_packet_length
//	cid = mch_make_simple_chunk(CHUNK_SHUTDOWN_COMPLETE,
//		FLAG_TBIT_SET);
//	simple_chunk_t_ptr_ = mch_complete_simple_chunk(cid);
//	ASSERT_EQ(ntohs(simple_chunk_t_ptr_->chunk_header.chunk_length), 8);
//	ASSERT_EQ(get_bundle_total_size(&default_bundle_ctrl_), 1480);
//	//  3.1 should force send && get_bundle_total_size == UDP_GECO_PACKET_FIXED_SIZES+8
//	mdi_bundle_ctrl_chunk(simple_chunk_t_ptr_, &path);
//	ASSERT_EQ(get_bundle_total_size(&default_bundle_ctrl_),
//		UDP_GECO_PACKET_FIXED_SIZES + 8);
//	mch_free_simple_chunk(cid);
//}

// last run and passed on 26 Agu 2016
//TEST(DISPATCHER_MODULE, test_recv_geco_packet)
//{
//	bool enable_0_if_recv_invalidate_packet_addr_port_length_integritycheck_and_so_on =
//		false;  // passed
//	bool enable_1_ifanINITACKCHUNKisreceived = false;  // passed
//	bool enable_2_if_recv_init_initack_shoutdowncomplete = false;  // passed
//	bool enable_3_if_recv_ABORT_CHUNK = false;  //passed
//	bool enable_4_if_recv_SHUTDOWN_ACK = false;  //passed
//	bool enable_5_if_recv_SHUTDOWN_COMPLETE = false;  //passed
//	bool enable_6_processinit_chunk = true; //passed
//	/////////////////////////////////////////////////////////////////////////////////////
//	ASSERT_EQ(sizeof(in_addr), 4);
//	ASSERT_EQ(sizeof(in6_addr), 16);
//	/////////////////////////////////////////////////////////////////////////////////////
//	init();
//	//disenable branch call  to test branch call this can reused all sample inputs to make life easier
//	enable_mock_dispatcher_disassemle_curr_geco_packet_ = true;
//	/////////////////////////////////////////////////////////////////////////////////////
//	//0)if an invalidate packet is received
//	if (enable_0_if_recv_invalidate_packet_addr_port_length_integritycheck_and_so_on)
//	{
//		sockaddrunion illegal_addr;
//		illegal_addr.sin.sin_family = AF_INET;
//		illegal_addr.sin.sin_port = 0;
//
//		// 0.1) if it is broadcast addr
//		illegal_addr.sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		//      0.1.1) should return recv_geco_packet_but_addrs_formate_check_failed
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			&illegal_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_addrs_formate_check_failed);
//		// 0.2) if it is any addr
//		illegal_addr.sin.sin_addr.s_addr = htonl(INADDR_ANY);
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		//      0.2.1) should return recv_geco_packet_but_addrs_formate_check_failed
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			&illegal_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_addrs_formate_check_failed);
//		// 0.3) if either dest port or src port is zero,
//		ushort oldport = geco_packet.pk_comm_hdr.dest_port;
//		geco_packet.pk_comm_hdr.dest_port = 0;
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		//      0.3.1) should return recv_geco_packet_but_addrs_formate_check_failed
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			&illegal_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_port_numbers_check_failed);
//		geco_packet.pk_comm_hdr.dest_port = oldport;
//		// 0.4) if geco packet len is not %4
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE + 1);
//		//      0.4.1) should return recv_geco_packet_but_integrity_check_failed
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet,
//			MIN_GECO_PACKET_SIZE + 1,
//			&illegal_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_integrity_check_failed);
//		// 0.5) if geco packet len < MIN_GECO_PACKET_SIZE
//		gset_checksum((char*)&geco_packet, 4);
//		//      0.5.1) should return recv_geco_packet_but_integrity_check_failed
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, 4, &illegal_addr,
//			&inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_integrity_check_failed);
//		// 0.6) if geco packet len  > MAX_GECO_PACKET_SIZE
//		gset_checksum((char*)&geco_packet, MAX_GECO_PACKET_SIZE + 1);
//		//       0.6.1) should return recv_geco_packet_but_integrity_check_failed
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet,
//			MAX_GECO_PACKET_SIZE + 1,
//			&illegal_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_integrity_check_failed);
//		// 0.7) if VALIDATION OF checksum not equals,
//		//      0.7.1) should return recv_geco_packet_but_integrity_check_failed
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			&illegal_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_integrity_check_failed);
//	}
//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	//1)if an INIT(ACK) CHUNK is received
//	if (enable_1_ifanINITACKCHUNKisreceived)
//	{
//		// fills up init chunk hdr
//		unsigned int initTag = htonl(generate_random_uint32());
//		unsigned int arwnd = 512;
//		unsigned short noOutStreams = 5;
//		unsigned short noInStreams = 5;
//		unsigned int initialTSN = initTag;
//		init_chunk_t* init_chunk = mch_make_init_chunk(initTag, rwnd, noOutStreams,
//			noInStreams, initialTSN);
//
//		for (i = 0; i < all_cannot_found_size; i++)
//		{  //1.1) if channel is NOT found as src addr not matched
//			last_src_addr = &all_cannot_found_addres[i];
//			for (uint j = 0; j < channel.local_addres_size; j++)
//			{
//				last_dest_addr = &channel.local_addres[j];
//				//1.2) but there is matched src addres in INIT chunk
//				//1.2.1) fills up init with matched addrlist
//				written = mch_write_vlp_addrlist(init_chunk->variableParams, src_addres,
//					srcaddr4_size);
//				written += mch_write_vlp_addrlist(&init_chunk->variableParams[written],
//					src_addres6, srcaddr6_size);
//				written += mch_write_vlp_supportedaddrtypes(
//					&init_chunk->variableParams[written], true, true, false);
//				dctp_packet_len = written + INIT_CHUNK_FIXED_SIZES
//					+ GECO_PACKET_FIXED_SIZE;
//				init_chunk->chunk_header.chunk_length = htons(
//					INIT_CHUNK_FIXED_SIZES + written);
//				memcpy(geco_packet.chunk, init_chunk,
//					dctp_packet_len - GECO_PACKET_FIXED_SIZE);
//				gset_checksum((char*)&geco_packet, dctp_packet_len);
//				ASSERT_EQ(INIT_CHUNK_FIXED_SIZES + written, 92);
//				ret = mdis_recv_geco_packet(0, (char*)dctp_packet, dctp_packet_len,
//					last_src_addr, last_dest_addr);
//				//1.2.2) should find an existed channel
//				ASSERT_EQ(ret, geco_return_enum::good);
//				ASSERT_EQ(curr_channel_, &channel);
//
//				//1.3) there is NO matched src addres in INIT chunk
//				//  1.3.1) fills up init with unmatched addrlist
//				written = 0;
//				dctp_packet_len = 0;
//				written += mch_write_vlp_supportedaddrtypes(init_chunk->variableParams,
//					true, true, false);
//				dctp_packet_len = written + INIT_CHUNK_FIXED_SIZES
//					+ GECO_PACKET_FIXED_SIZE;
//				init_chunk->chunk_header.chunk_length = htons(
//					INIT_CHUNK_FIXED_SIZES + written);
//				memcpy(geco_packet.chunk, init_chunk,
//					dctp_packet_len - GECO_PACKET_FIXED_SIZE);
//				gset_checksum((char*)&geco_packet, dctp_packet_len);
//				ASSERT_EQ(INIT_CHUNK_FIXED_SIZES + written, 28);
//				ret = mdis_recv_geco_packet(0, (char*)dctp_packet, dctp_packet_len,
//					last_src_addr, last_dest_addr);
//				//1.3.2) should NOT find an existed channel
//				ASSERT_EQ(ret, geco_return_enum::good);
//				ASSERT_EQ(curr_channel_, nullptr);
//			}
//		}
//	}
//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	//2) if recv init, init ack or shutdown complete chunks
//	if (enable_2_if_recv_init_initack_shoutdowncomplete)
//	{
//		//2.1) if they are not the only one chunk in the packet
//		// here we add another data chunk
//		last_src_addr = &channel.remote_addres[0];
//		last_dest_addr = &channel.local_addres[0];
//		uchar chunktypes[3] =
//		{ CHUNK_INIT_ACK, CHUNK_INIT, CHUNK_SHUTDOWN_COMPLETE };
//		uint reterrnos[3] =
//		{ recv_geco_packet_but_morethanone_init_ack,
//			recv_geco_packet_but_morethanone_init,
//			recv_geco_packet_but_morethanone_shutdown_complete };
//
//		((chunk_fixed_t*)&geco_packet.chunk[4])->chunk_id = CHUNK_DATA;
//		((chunk_fixed_t*)&geco_packet.chunk[4])->chunk_flags = 0;
//		((chunk_fixed_t*)&geco_packet.chunk[4])->chunk_length = htons(4);
//		dctp_packet_len = 2 * CHUNK_FIXED_SIZE + GECO_PACKET_FIXED_SIZE;
//
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_flags = 0;
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_length = htons(4);
//		for (int i = 0; i < 3; ++i)
//		{
//			((chunk_fixed_t*)geco_packet.chunk)->chunk_id = chunktypes[i];
//			//2.2) the packet should be discarded
//			gset_checksum((char*)&geco_packet, dctp_packet_len);
//			ret = mdis_recv_geco_packet(0, (char*)dctp_packet, dctp_packet_len,
//				last_src_addr, last_dest_addr);
//			ASSERT_EQ(ret, (uint)reterrnos[i]);
//		}
//	}
//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	//3) if recv ABORT CHUNK
//	if (enable_3_if_recv_ABORT_CHUNK)
//	{
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_id = CHUNK_ABORT;
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_flags = 0;
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_length = htons(4);
//		//3.1 if ootb packet
//		last_src_addr = &all_cannot_found_addres[0];
//		//  3.1.1 should discard
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_it_is_ootb_abort_discard);
//		//3.2 if non-ootb packet
//		last_src_addr = &channel.remote_addres[0];
//		//  3.2.1)if tbit set && last_veri_tag_ == curr_channel_->remote_tag
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_flags = 0x01;
//		geco_packet.pk_comm_hdr.verification_tag = htonl(channel.remote_tag);
//		//      3.2.1.1) should go on
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret, good);
//		ASSERT_EQ(is_found_abort_chunk_, true);
//		//  3.2)if !is_tbit_set && last_veri_tag_ == curr_channel_->local_tag)
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_flags = 0;
//		geco_packet.pk_comm_hdr.verification_tag = htonl(channel.local_tag);
//		//      3.2.1) should go on
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret, good);
//		ASSERT_EQ(is_found_abort_chunk_, true);
//		//  3.3)if tbit and verifi tag not matched
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_flags = 0;
//		geco_packet.pk_comm_hdr.verification_tag = htonl(channel.remote_tag);
//		//  3.3.1)should discard
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret,
//			recv_geco_packet_but_nootb_abort_chunk_has_ielegal_verifi_tag);
//	}
//	/////////////////////////////////////////////////////////////////////////////////////
//	//4) if recv SHUTDOWN_ACK
//	if (enable_4_if_recv_SHUTDOWN_ACK)
//	{
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_id = CHUNK_SHUTDOWN_ACK;
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_flags = 0;
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_length = htons(4);
//		geco_packet.pk_comm_hdr.verification_tag = htonl(channel.local_tag);
//
//		//3.1 if ootb packet
//		last_src_addr = &all_cannot_found_addres[0];
//		//  3.1.1 should discard
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_it_is_ootb_sdack_send_sdc);
//		//3.2 if non-ootb packet
//		last_src_addr = &channel.remote_addres[0];
//		//  3.2.1)if curr channel state is neither cookie echoed nor cookie wait
//		channel.state_machine_control->channel_state = ChannelState::ShutdownSent;
//		//      3.2.1.1) should go on
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret, good);
//		//  3.2.2)if curr channel state is either cookie echoed or cookie wait
//		channel.state_machine_control->channel_state = ChannelState::CookieWait;
//		//      3.2.2.1) should send shutdown complete chunk to the peer
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret, discard);
//		//  3.2.3)if veri tag unmatched channel local tag
//		geco_packet.pk_comm_hdr.verification_tag = htonl(channel.local_tag - 1);
//		channel.state_machine_control->channel_state = ChannelState::ShutdownSent;
//		//      3.2.3.1) should discard
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_nootb_packet_verifitag_illegal);
//	}
//	/////////////////////////////////////////////////////////////////////////////////////
//	//5) if recv SHUTDOWN_COMPLETE
//	if (enable_5_if_recv_SHUTDOWN_COMPLETE)
//	{
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_id =
//			CHUNK_SHUTDOWN_COMPLETE;
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_flags = 0;
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_length = htons(4);
//		geco_packet.pk_comm_hdr.verification_tag = htonl(channel.local_tag);
//
//		//4.1 if ootb packet
//		last_src_addr = &all_cannot_found_addres[0];
//		//  4.1.1 should discard
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &inst.local_addres_list[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_it_is_ootb_sdc_discard);
//		//4.2 if non-ootb packet
//		last_src_addr = &channel.remote_addres[0];
//		//  4.2.1)if curr channel state is not ShutdownAckSent
//		channel.state_machine_control->channel_state = ChannelState::Connected;
//		//      4.2.1.1) should discard
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret,
//			recv_geco_packet_but_nootb_sdc_recv_otherthan_sdc_ack_sentstate);
//		//  4.2.2)if curr channel state is ShutdownAckSent
//		channel.state_machine_control->channel_state =
//			ChannelState::ShutdownAckSent;
//		//      4.2.2.1) if verifi tag matched
//		((chunk_fixed_t*)geco_packet.chunk)->chunk_flags = 0;
//		geco_packet.pk_comm_hdr.verification_tag = htonl(channel.local_tag);
//		//          4.2.2.2 should go on
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret, good);
//		//  3.2.3)if veri tag unmatched
//		geco_packet.pk_comm_hdr.verification_tag = htonl(channel.local_tag - 1);
//		//      3.2.3.1) should discard
//		gset_checksum((char*)&geco_packet, MIN_GECO_PACKET_SIZE);
//		ret = mdis_recv_geco_packet(0, (char*)dctp_packet, MIN_GECO_PACKET_SIZE,
//			last_src_addr, &channel.local_addres[0]);
//		ASSERT_EQ(ret, recv_geco_packet_but_nootb_sdc_recv_verifitag_illegal);
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	unsigned int initTag = htonl(generate_random_uint32());
//	unsigned int arwnd = 512;
//	unsigned short noOutStreams = 5;
//	unsigned short noInStreams = 5;
//	unsigned int initialTSN = initTag;
//	init_chunk_t* init_chunk = mch_make_init_chunk(initTag, rwnd, noOutStreams,
//		noInStreams, initialTSN);
//
//	if (enable_6_processinit_chunk)
//	{
//		reset_geco_packet_fixed()
//			;
//
//		enable_mock_dispatcher_disassemle_curr_geco_packet_ = false;
//		enable_mock_dispatcher_process_init_chunk_ = false;
//
//		//1.1) if channel is NOT found as src addr not matched
//		last_src_addr = &all_cannot_found_addres[0];
//		last_dest_addr = &channel.local_addres[0];
//		//　there is NO matched src addres in INIT chunk
//		//   fills up init with unmatched addrlist
//		written = 0;
//		dctp_packet_len = 0;
//		//    written = put_vlp_addrlist(init_chunk->variableParams, all_cannot_found_addres,
//		//            all_cannot_found_size);
//		written += mch_write_vlp_supportedaddrtypes(
//			&init_chunk->variableParams[written], true, true, false);
//		dctp_packet_len = written + INIT_CHUNK_FIXED_SIZES + GECO_PACKET_FIXED_SIZE;
//		init_chunk->chunk_header.chunk_length = htons(
//			INIT_CHUNK_FIXED_SIZES + written);
//		memcpy(geco_packet.chunk, init_chunk,
//			dctp_packet_len - GECO_PACKET_FIXED_SIZE);
//		gset_checksum((char*)&geco_packet, dctp_packet_len);
//		printf(
//			"..............................................test branch call disassemble's branch call process_init_chunk()...................................\n");
//		//ret = mdis_recv_geco_packet(0, (char*)dctp_packet, dctp_packet_len,last_src_addr, last_dest_addr);
//	}
//
//}

// passed on 28/02/2017
TEST(DISPATCHER_MODULE, test_contains_chunk)
{
  /**
   * contains_chunk: looks for chunk_type in a newly received geco packet
   * Should be called after find_chunk_types().
   * The chunkArray parameter is inspected. This only really checks for chunks
   * with an ID <= 30. For all other chunks, it just guesses...
   * @return 0 NOT contains, 1 contains and only one, 2 contains and NOT only one
   * @pre: need call find_chunk_types() first
   */
  uint chunk_types;
  //////////////////////////////////////////////////////////////////////////////
  chunk_types = 0;
  ASSERT_EQ(contains_chunk(CHUNK_DATA, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_SACK, chunk_types), 0);
  ASSERT_EQ(contains_chunk(CHUNK_HBREQ, chunk_types), 0);
  //////////////////////////////////////////////////////////////////////////////
  // INIT must be the only chunk in the packet
  chunk_types = 0;
  chunk_types |= 1 << CHUNK_INIT;
  ASSERT_EQ(contains_chunk(CHUNK_INIT, chunk_types), 1);
  //////////////////////////////////////////////////////////////////////////////
  chunk_types = 0;
  chunk_types |= 1 << CHUNK_DATA;
  chunk_types |= 1 << CHUNK_SACK;
  chunk_types |= 1 << CHUNK_HBREQ;
  ASSERT_EQ(contains_chunk(CHUNK_DATA, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_SACK, chunk_types), 2);
  ASSERT_EQ(contains_chunk(CHUNK_HBREQ, chunk_types), 2);
  //////////////////////////////////////////////////////////////////////////////
}

