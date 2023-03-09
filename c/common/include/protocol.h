/*
 * protocol.h
 *
 *  Created on: 2023年3月3日
 *      Author: hou-lei
 */

#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

#include "wstring.h"

/**
 * max length of chat package
 */
#define CHAT_PACK_MAX_LEN (8*1024)

typedef enum ChatPackageVersion {
	// ByteOrder + Version
	V01=0xFEFF0001,
	V11=0xFEFF0101
} ChatPackageVersion;

typedef enum ChatPackageType {
	HeartBeatRequest	= 0x00000001,
	HeartBeatResponse	= 0x80000001,
	RegistRequest		= 0x00000002,
	RegistResponse		= 0x80000002,
	LoginRequest		= 0x00000003,
	LoginResponse		= 0x80000003,
	ListFriendsRequest	= 0x00000004,
	ListFriendsResponse	= 0x80000004,
	MakeFriendsRequest	= 0x00000005,
	MakeFriendsResponse	= 0x80000005,
	SendMessageRequest	= 0x00000006,
	SendMessageResponse	= 0x80000006

} ChatPackageType;

typedef enum ErrorCode {
	EC_OK = 0,
	EC_WrongLoginName = 1,
	EC_WrongLodinPass = 2,

} ErrorCode;

/**
 * header
 */
typedef struct ChatHeader {
	ChatPackageVersion version;
	int sequence;
	ChatPackageType type;
	int length;
} ChatHeader;

/**
 * body with no data
 */
typedef struct CB_None {
	// no body
} CB_None;

/**
 *
 */
typedef struct CB_RegistRequest {
	wstring loginName;
	wstring loginPass;
	wstring nickName;
} CB_RegistRequest;

typedef struct CB_RegistResponse {
	ErrorCode errorCode;
	wstring message;
	int userId;
} CB_RegistResponse;

typedef struct CB_LoginRequest {
	wstring loginName;
	wstring loginPass;
} CB_LoginRequest;

typedef struct CB_ChatUser {
	int userId;
	wstring nickName;
} CB_ChatUser;

typedef struct CB_LoginResponse {
	ErrorCode errorCode;
	wstring message;
	CB_ChatUser user;
} CB_LoginResponse;

typedef struct CB_ListFriendsRequest {
	int start;
	int display;
} CB_ListFriendsRequest;

typedef struct CB_ListFriendsResponse {
	ErrorCode errorCode;
	wstring message;
	int totalCount;
	int listSize;
	CB_ChatUser * friends;
} CB_ListFriendsResponse;

typedef struct CB_MakeFriendsRequest {
	CB_ChatUser friend;
} CB_MakeFriendsRequest;

typedef struct CB_MakeFriendsResponse {
	ErrorCode errorCode;
	wstring message;
	CB_ChatUser friend;
} CB_MakeFriendsResponse;

typedef struct CB_SendMessageRequest {
	CB_ChatUser user;
	wstring message;
} CB_SendMessageRequest;

typedef struct CB_SendMessageResponse {
	ErrorCode errorCode;
	wstring message;
} CB_SendMessageResponse;


typedef union ChatBody {
	CB_None* none;
	CB_RegistRequest*	registRequest;
	CB_RegistResponse*	registResponse;
	CB_LoginRequest	*	loginRequest;
	CB_LoginResponse*	loginResponse;
	CB_ListFriendsRequest*	listFriendsRequest;
	CB_ListFriendsResponse*	listFriendsResponse;
	CB_MakeFriendsRequest*	makeFriendsRequest;
	CB_MakeFriendsResponse*	makeFriendsResponse;
	CB_SendMessageRequest*	sendMessageRequest;
	CB_SendMessageResponse*	sendMessageResponse;

} ChatBody;

typedef struct ChatPackage {
	ChatHeader header;
	ChatBody body;
} ChatPackage;



typedef enum CodecErrorCode {
	Codec_OK = 0,

} CodecErrorCode;

CodecErrorCode encode(ChatPackage * target, void * source);

CodecErrorCode decode(void * target, ChatPackage * source);

#endif /* __PROTOCOL_H_ */
