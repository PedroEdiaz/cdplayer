#include <fcntl.h> 
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#include <tinyalsa/pcm.h>

#define FRAMES 1

#define SIZE CD_FRAMESIZE_RAW
#define PERIOD 1024
#define COUNT 2

struct cdrom_read_audio sr;

void forward( int signum )
{
	sr.addr.lba+=3600;
}

void backward( int signum )
{
	sr.addr.lba-=3600;
}

void weite_err( char * s )
{
	char * p;
	p=s;
	while( *p )
		++p;
	write( STDERR_FILENO, s, p-s );
}

int main( int argc, char * argv[] )
{
	int fdi, card=0, device=0;
	struct pcm * pcm;
	struct pcm_config config;
	char *msg, buff[SIZE*FRAMES];
	struct sigaction f_action, b_action;

	if( argc < 2 )
	{
		weite_err( "Usage: " );
		weite_err( argv[0] );
		weite_err( " /dev/sr0 (<card>) (<device>)\n" );
		goto err0;
	}

	if( (fdi=open( argv[1] , O_RDONLY | O_NONBLOCK )) < 0 )
	{
		weite_err( "couldn't open input device " );
		weite_err( argv[1] );
		weite_err( " \n" );
		goto err0;
	}

	switch( ioctl( fdi, CDROM_DRIVE_STATUS, 0 ) )
	{
	case CDS_NO_DISC:
		weite_err( "No disk\n" );
		goto err1;
	case CDS_DRIVE_NOT_READY:
		weite_err( "Drive not ready\n" );
		goto err1;
	case CDS_TRAY_OPEN:
		weite_err( "Tray open\n" );
		goto err1;
	case -1:
		weite_err( "Error\n" );
		goto err1;
	}

	switch( argc )
	{
	case 4:
		device = *argv[3] - '0';
	case 3:
		card = *argv[2] - '0';
	}

	/* Prepare signals */
	{
		f_action.sa_handler = forward ; 
		b_action.sa_handler = backward ; 

		sigaddset( &f_action.sa_mask, SIGUSR1 ); 
		sigaddset( &b_action.sa_mask, SIGUSR2 ); 
		sigaction( SIGUSR1, &f_action, NULL );
		sigaction( SIGUSR2, &b_action, NULL );
	}

	/* config pcm */
	{
		config.channels = 2;
		config.rate = 44100;
		config.format = PCM_FORMAT_S16_LE;

		config.period_size = PERIOD;
		config.period_count = COUNT;
		config.start_threshold = PERIOD;
		config.silence_threshold = PERIOD*COUNT;
		config.stop_threshold = PERIOD*COUNT;
	}

	/* config sr */
	{
		sr.buf = buff;
		sr.nframes = FRAMES;
		sr.addr_format = CDROM_LBA;
	}

	pcm = pcm_open( card, device, PCM_OUT , &config );

	sr.addr.lba=0;
	while( ioctl( fdi, CDROMREADAUDIO, &sr ) != -1 )
	{
		pcm_writei( pcm, buff, SIZE*FRAMES/4 );
		sr.addr.lba+=FRAMES; 
	}

	pcm_close( pcm );
	close( fdi );
	return 0;
err1:
	close( fdi );
err0:
	return 1;
}
