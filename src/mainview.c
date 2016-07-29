#include "myevic.h"
#include "screens.h"
#include "display.h"
#include "battery.h"
#include "myrtc.h"
#include "atomizer.h"
#include "miscs.h"
#include "M451Series.h"

//=============================================================================

unsigned char	ShowDateFlag = 0;

//=============================================================================
//----- (00001654) --------------------------------------------------------
__myevic__ void MainView()
{
	if ( Screen != 1 || !PD3 || !PD2 || !PE0 )
	{
		HideLogo = 3;
	}
	Flags64 |= 0x20000;
	Screen = 1;
	ScreenDuration = 30;
}


//=============================================================================
__myevic__ void DrawMode()
{
	if ( BLINKITEM(0) )
		return;

	switch ( dfMode )
	{
		case 0:
			DrawString( String_TEMP, 0, 2 );
			if ( !BLINKITEM(1) )
			{
				DrawString( String_NI, 32, 2 );
			}
			break;
		case 1:
			DrawString( String_TEMP, 0, 2 );
			if ( !BLINKITEM(1) )
			{
				DrawString( String_TI, 32, 2 );
			}
			break;
		case 2:
			DrawString( String_TEMP, 0, 2 );
			if ( !BLINKITEM(1) )
			{
				DrawString( String_SS, 32, 2 );
				DrawImage( 48, 3, 0x04 );
				DrawImage( 54, 3, 0x02 );
				DrawImage( 59, 3, 0x07 );
			}
			break;
		case 3:
			DrawString( String_TCR, 0, 2 );
			DrawValue( 25, 2, (&dfTCRM1)[(int)dfTCRIndex], 0, 0x0B, 3 );
			if ( !BLINKITEM(1) )
			{
				DrawImage( 49, 2, 0xA8 );
				DrawValue( 57, 2, dfTCRIndex + 1, 0, 0x0B, 1 );
			}
			break;
		case 4:
			DrawString( String_POWER, 0, 2 );
			break;
		case 5:
			DrawString( String_BYPASS, 0, 2 );
			break;
		case 6:
			DrawString( String_START, 13, 4 );
			break;
		default:
			break;
	}
}

//=============================================================================

__myevic__ void DrawPwrLine( int pwr, int line )
{
	if ( BLINKITEM(2) && PD2 && PD3 )
		return;

	if ( Anim3d )
	{
		if ( Screen == 1 && !EditModeTimer )
			return;
	}

    DrawString( String_PWR_s, 0, line+2 );

	if ( pwr < 100 )
	{
		DrawValue( 27, line, pwr, 1, 0x1F, 2 );
		DrawImage( 48, line+2, 0x98 );
	}
	else if ( pwr < 1000 )
	{
		DrawValue( 25, line, pwr, 1, 0x1F, 3 );
		DrawImage( 54, line+2, 0x98 );
	}
	else
	{
		if ( ( !PD3 || !PD3 ) && EditItemIndex == 2 )
		{
			DrawValue( 25, line, pwr, 1, 0x1F, 4 );
		}
		else
		{
			DrawValue( 27, line, pwr / 10, 0, 0x1F, 3 );
			DrawImage( 53, line+2, 0x98 );
		}
	}
}

//=============================================================================

__myevic__ void DrawCoilLine( int line )
{
	unsigned int rez;

	if ( Anim3d )
	{
		if ( Screen == 1 && !EditModeTimer )
			return;
	}

	if ( myDbgFlag & 1 )
	{
		if ( !EditModeTimer || EditItemIndex != 3 )
		{
			DrawHexLong( 0, line, Flags64, 1 );
			return;
		}
	}

	if ( BLINKITEM(3) )
		return;

	DrawString( String_COIL_s, 0, line+2 );

	if ( Flags64 & 0x100 )
	{
		rez = AtoRezMilli / 10u;
	}
	else if ( ISMODETC(dfMode) )
	{
		if ( byte_200000B3 || !AtoRez )
		{
			rez = AtoRez;
		}
		else
		{
			rez = dfResistance;
		}
	}
	else
	{
		rez = AtoRez;
	}

	DrawValue( 27, line, rez, 2, 0x1F, 3 );

	if ((( dfMode == 0 ) && ( dfRezLockedNI ))
	||	(( dfMode == 1 ) && ( dfRezLockedTI ))
	||	(( dfMode == 2 ) && ( dfRezLockedSS ))
	||	(( dfMode == 3 ) && ( dfRezLockedTCR )))
	{
		DrawImage( 56, line+2, 0xC3 );
	}
	else
	{
		DrawImage( 56, line+2, 0xC0 );
	}
}

//=============================================================================

__myevic__ void DrawAPTLine( int line )
{
	if ( BLINKITEM(4) )
		return;

	if ( Anim3d )
	{
		if ( Screen == 1 && !EditModeTimer )
			return;
	}

	switch ( dfAPT )
	{
		case 0:
		default:
		{
			DrawString( String_AMP_s, 0, line+2 );
			DrawValue( 27, line, ( Flags64 & 0x100 ) ? AtoCurrent : 0, 1, 0x1F, 3 );
			DrawImage( 56, line+2, 0x9C );
			break;
		}

		case 1:
		{
			DrawString( String_PUFF_s, 0, line+2 );
			DrawValue( 24, line, dfPuffCount, 0, 0x1F, 5 );
			break;
		}

		case 2:
		{
			DrawString( String_TIME_s, 0, line+2 );
			DrawValue( 24, line, dfTimeCount / 10, 0, 0x1F, 5 );
			break;
		}

		case 3:
		{
			DrawString( String_BATT_s, 0, line+2 );
			DrawValue( 27, line, ((Flags64&0x100)&&RTBatVolts)?RTBatVolts:BatteryVoltage, 2, 0x1F, 3 );
			DrawImage( 57, line+2, 0x97 );
			break;
		}
		
		case 4:
		{
			DrawString( String_VOUT_s, 0, line+2 );
			DrawValue( 27, line, (Flags64&0x100)?AtoVolts:0, 2, 0x1F, 3 );
			DrawImage( 57, line+2, 0x97 );
			break;
		}
		
		case 5:
		{
			DrawString( String_TEMP_s, 0, line+2 );
			if ( dfIsCelsius )
			{
				DrawValue( 39, line, BoardTemp, 0, 0x1F, 2 );
				DrawImage( 57, line-1, 0x6A );
			}
			else
			{
				int t = CelsiusToF( BoardTemp );
				DrawValue( t>99?31:39, line, CelsiusToF( BoardTemp ), 0, 0x1F, 3 );
				DrawImage( 57, line-1, 0x6D );
			}
			break;
		}

		case 6:
		{
			S_RTC_TIME_DATA_T rtd;
			GetRTC( &rtd );
			if ( ShowDateFlag )
			{
				DrawDate( 4, line, &rtd, 0x1F );
			}
			else
			{
				DrawTime( 5, line, &rtd, 0x1F );
			}
			break;
		}
	}
}

//=============================================================================

__myevic__ void ShowMainFiring()
{
  int tempc;

	DrawMode();

	switch ( dfMode )
	{
		case 0:
		case 1:
		case 2:
		case 3:
		{
			if ( dfIsCelsius )
			{
				tempc = FarenheitToC( AtoTemp );
				if ( dfTemp <= tempc )
				{
					DrawString( String_Protec, 2, 20 );
				}
				else
				{
					DrawValue( 0, 13, tempc, 0, 72, 3 );
					DrawImage( 48, 20, 0xE0 );
				}
			}
			else
			{
				if ( dfTemp <= AtoTemp )
				{
					DrawString( String_Protec, 2, 20 );
				}
				else
				{
					DrawValue( 0, 13, AtoTemp, 0, 72, 3 );
					DrawImage( 48, 20, 0xE1 );
				}
			}
			DrawPwrLine( AtoPower( TargetVolts ), 52 );
			break;
		}

		case 4:
		case 5:
		{
			DrawValue( 10, 49, FireDuration, 1, 41, 2 );
			DrawImage( 40, 49, 0xB7 );
			break;
		}

		case 6:
		default:
			break;
	}
}


//=============================================================================
//----- (000068A0) --------------------------------------------------------
__myevic__ void DrawBFLine( int y )
{
	for ( int v = 0 ; v < 13 ; ++v )
	{
		DrawHLine( 5 * v, y, 5 * v + 2, 1 );
		DrawHLine( 5 * v, y + 1, 5 * v + 2, 1 );
	}
}


//=============================================================================

__myevic__ void ShowMainView()
{
	unsigned int pwr; // r2@2
	unsigned int v15; // r0@93
	unsigned int i, j;
	unsigned int v17; // r8@98
	unsigned int v19; // r3@99
	unsigned int v20; // r1@99
	unsigned int v26; // r2@168
	int v27; // r3@169

	if ( dfUIVersion != 2 )
		return;

	pwr = dfPower;

	if ( dfMode == 5 )
	{
		if ( Flags64 & 0x100 )
		{
			pwr = AtoPower( AtoVolts );
		}
		else
		{
			pwr = ClampPower( BatteryVoltage, 0 );
		}
	}
	if ( Screen == 2 )
	{
		pwr = pwr * PowerScale / 100;
	}

	DrawMode();

	if ( dfMode < 4 )
	{
		if ( Screen == 2 )
		{
			ShowMainFiring();
		}
		else
		{
			DrawValue( 0, 13, dfTemp, 0, 0x48, 3 );
			DrawImage( 48, 20, dfIsCelsius ? 0xE0 : 0xE1 );

			if ( !HideLogo ) DrawLOGO( 0, 0 );

			DrawPwrLine( dfTCPower, 52 );
		}
	}

	if ( dfMode == 4 )
	{
		if ( pwr > 999 )
		{
			DrawValue( 0, 18, pwr, 1, 0x29, 4 );
			DrawImage( 54, 26, 0x98 );
		}
		else if ( pwr > 99 )
		{
			DrawValue( 0, 13, pwr, 1, 0x48, 3 );
			DrawImage( 54, 26, 0x98 );
		}
		else
		{
			DrawValue( 5, 13, pwr, 1, 0x48, 2 );
			DrawImage( 45, 18, 0xB9 );
		}

		if ( !HideLogo ) DrawLOGO( 0, 0 );

		if ( Screen != 2 )
		{
			DrawString( String_VOLT_s, 0, 54 );
			DrawValue( 27, 52, dfVWVolts, 2, 0x1F, 3 );
			DrawImage( 57, 54, 0x97 );
		}
		else
		{
			ShowMainFiring();
		}
	}

	if ( dfMode == 5 )
	{
		if ( Flags64 & 0x100 )
		{
			DrawValue( 0, 13, AtoVolts, 2, 0x48, 3 );
			DrawImage( 54, 26, 0x97 );
		}
		else
		{
			if ( pwr < 100 )
			{
				DrawValue( 5, 13, pwr, 1, 0x48, 2 );
				DrawImage( 45, 18, 0xB9 );
			}
			else if ( pwr < 1000 )
			{
				DrawValue( 0, 13, pwr, 1, 0x48, 3 );
				DrawImage( 54, 26, 0x98 );
			}
			else
			{
				DrawValue( 0, 18, pwr, 1, 0x29, 4 );
				DrawImage( 54, 26, 0x98 );
			}
		}

		if ( !HideLogo ) DrawLOGO( 0, 0 );

		if ( Screen != 2 )
		{
			DrawString( String_VOLT_s, 0, 54 );
			DrawValue( 27, 52, BatteryVoltage, 2, 0x1F, 3 );
			DrawImage( 57, 54, 0x97 );
		}
		else
		{
			ShowMainFiring();
		}
	}

	if ( dfMode == 6 )
	{
		DrawString( String_MAX_s, 23, 15 );
		DrawString( String_MIN_s, 23, 97 );
		DrawHLine( 0, 18, 21, 1 );
		DrawHLine( 43, 18, 63, 1 );
		DrawHLine( 0, 100, 21, 1 );
		DrawHLine( 43, 100, 63, 1 );

		v15 = SearchSTARTRez( dfSavedCfgRez[(int)ConfigIndex] );
		if ( v15 > 3 )
		{
			for ( i = 0;
				  dfSavedCfgPwr[(int)ConfigIndex] / ( MaxPower / 15 ) > i;
				  ++i )
			{
				DrawFillRect( 0, 94 - 5*i, 63, 95 - 5*i, 1 );
			}
			if ( !i ) DrawBFLine( 94 );
		}
		else
		{
			v20 = STARTPowers[ 2 * v15 + 1 ] / (MaxPower / 15u);
			v17 = 99 - 5 * v20;
			for ( j = 0; ; ++j )
			{
				v19 = dfSavedCfgPwr[(int)ConfigIndex] / (MaxPower / 15u);
				if ( v19 <= j )
					break;
				if ( v19 >= v20 && v20 - 1 <= j )
					DrawBFLine( 94 - 5*j );
				else
					DrawFillRect( 0, 94 - 5*j, 63, 95 - 5*j, 1 );
			}
			if ( !j ) DrawBFLine(94);
			DrawBFLine( v17 );
			DrawHLine( 25, v17, 40, 0 );
			DrawHLine( 25, v17 + 1, 40, 0 );
			DrawString( String_BF_s, 29, v17 - 3 );
		}

		if ( !ShowWeakBatFlag )
		{
			if ( !( Flags64 & 0x100 ) )
			{
				if ( !PD2 || !PD3 )
				{
					v26 = dfSavedCfgPwr[(int)ConfigIndex];
					if ( v26 >= 1000 )
					{
						v26 /= 10u;
						v27 = 0;
					}
					else
					{
						v27 = 1;
					}
					DrawValue( 11, 112, v26, v27, 0x1F, 3 );
					DrawImage( 42, 114, 0xB2 );
				}
				else
				{
					ShowBattery();
				}
			}
			else
			{
				DrawValue( 10, 110, FireDuration, 1, 41, 2 );
				DrawImage( 40, 110, 0xB7 );
			}
		}
	}

	if ( dfMode != 6 )
	{
		DrawHLine( 0, 43, 63, 1 );
		DrawHLine( 0, 107, 63, 1 );

		DrawCoilLine( 71 );
		DrawAPTLine( 90 );

		if ( Anim3d )
		{
			if ( Screen == 1 && !EditModeTimer )
			{
				anim3d( 1 );
			}
		}

		ShowBattery();
	}

	if ( ShowWeakBatFlag )
	{
		DrawFillRect( 0, 107, 63, 127, 0 );
		ShowWeakBat();
	}
}
