#include "kek.h"
#include "AL/al.h"
#include "AL/alc.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct riff_header {
	uint8_t chunk_id[4];
	uint32_t chunk_size;
	uint8_t format[4];
} RIFFHeader;

typedef struct wav_format {
	uint8_t sub_chunk_id[4];
	uint32_t sub_chunk_size;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;
} WAVFormat;

typedef struct wav_data {
	uint8_t sub_chunk_id[4];
	uint32_t sub_chunk2_size;
} WAVData;

typedef struct buffer {
    int id;
    ALuint buffer;
} Buffer;

typedef struct source {
    ALuint source;
    bool used;
} Source;

typedef struct sound {
    int id;
    Buffer *buffer;
    Source *source;
    float pitch;
    float gain;
    bool loop;
} Sound;

// the openal code might need to be moved to the PAL layer
static ALCdevice *device = NULL;
static ALCcontext *context = NULL;
static Source *sources = NULL;
static size_t source_count = 0;
static MemPool buffer_pool;
static MemPool sound_pool;

static Source *take_source(void);
static void release_source(Source *source);
static Buffer *get_sound_buffer(int bufferid);
static Sound *get_sound(int sound);

int init_sound(size_t num_sources, size_t buffer_capacity, size_t sound_capacity)
{
	ALfloat orientation[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	device = alcOpenDevice(NULL);

	context = alcCreateContext(device, NULL);

	if (!alcMakeContextCurrent(context)) 
		return KEK_ERROR;

	alListenerfv(AL_ORIENTATION, orientation);

    source_count = num_sources;
    sources = memstack_push(sizeof(Source) * num_sources);
    for(size_t i = 0; i < num_sources; ++i)
    {
        alGenSources(1U, &sources[i].source);
        sources[i].used = false;
    }

    mempool_alloc(&buffer_pool, buffer_capacity, sizeof(Buffer));
    mempool_alloc(&sound_pool, sound_capacity, sizeof(Sound));

	return KEK_OK;
}

void update_sounds(void)
{
    // add two day dependency between source and attached sound
    // traverse trhough all sounds and release the free ones
}

int create_sound_buffer(void)
{
    Buffer *buffer = mempool_take(&buffer_pool);
    buffer->id = mempool_get_slot(&buffer_pool, buffer);
    
	alGenBuffers(1U, &buffer->buffer);

    return buffer->id;
}

bool load_wav_file_to_sound_buffer(int bufferid, const char *file)
{
	FILE *fh = NULL;
	WAVFormat format;
	RIFFHeader header;
	WAVData data;
	uint8_t *payload = NULL;
	ALsizei size = 0;
	ALsizei frequency = 0;
	ALenum format_type = AL_INVALID_ENUM;

	fh = fopen(file, "rb");

	if (!fh)
		return false;

	if (fread(&header, sizeof(header), 1, fh) == 0) {
		fclose(fh);
		return false;
	}

	if ((header.chunk_id[0] != 'R' || header.chunk_id[1] != 'I' ||
	     header.chunk_id[2] != 'F' || header.chunk_id[3] != 'F') ||
	    (header.format[0] != 'W' || header.format[1] != 'A' ||
	     header.format[2] != 'V' || header.format[3] != 'E')) {
		fclose(fh);
		return false;
	}

	if (fread(&format, sizeof(format), 1, fh) == 0) {
		fclose(fh);
		return false;
	}

	if (format.sub_chunk_id[0] != 'f' || format.sub_chunk_id[1] != 'm' ||
	    format.sub_chunk_id[2] != 't' || format.sub_chunk_id[3] != ' ') {
		fclose(fh);
		return false;
	}

	if (format.sub_chunk_size > 16) {
		fseek(fh, sizeof(short), SEEK_CUR);
	}

	if (fread(&data, sizeof(data), 1, fh) == 0) {
		fclose(fh);
		return false;
	}

	//check for data tag in memory
	if (data.sub_chunk_id[0] != 'd' || data.sub_chunk_id[1] != 'a' ||
	    data.sub_chunk_id[2] != 't' || data.sub_chunk_id[3] != 'a') {
		fclose(fh);
		return false;
	}

	payload = memstack_push(data.sub_chunk2_size * sizeof(uint8_t));
	if (!fread(payload, data.sub_chunk2_size, 1, fh)) {
		fclose(fh);
        memstack_pop(payload);
		return false;
	}

	size = data.sub_chunk2_size;
	frequency = format.sample_rate;

	if (format.num_channels == 1) {
		if (format.bits_per_sample == 8) {
			format_type = AL_FORMAT_MONO8;
		}
		else if (format.bits_per_sample == 16) {
			format_type = AL_FORMAT_MONO16;
		}
	}
	else if (format.num_channels == 2) {
		if (format.bits_per_sample == 8) {
			format_type = AL_FORMAT_STEREO8;
		}
		else if (format.bits_per_sample == 16) {
			format_type = AL_FORMAT_STEREO16;
		}
	}
	
    Buffer *buffer = get_sound_buffer(bufferid);
	alBufferData(buffer->buffer, format_type, payload, size, frequency);
	fclose(fh);

    memstack_pop(payload);

	return true;
}

int create_sound(int bufferid)
{
    Buffer *buffer = get_sound_buffer(bufferid);
    Sound *sound = mempool_take(&sound_pool);
    sound->buffer = buffer;
    sound->source = NULL;
    sound->pitch = 1.0f;
    sound->gain = 1.0f;
    sound->loop = false;

    sound->id = mempool_get_slot(&sound_pool, sound);

    return sound->id;
}

void play_sound(int soundid)
{
    Sound *sound = get_sound(soundid);

    if(!sound->source)
        sound->source = take_source();

    if(sound->source)
    {
        ALuint alsrc = sound->source->source;
        ALuint albuf = sound->buffer->buffer;

	    ALint loop = sound->loop ? AL_TRUE : AL_FALSE;
	    alSourcef(alsrc, AL_GAIN, sound->gain);
	    alSourcef(alsrc, AL_PITCH, sound->pitch);
	    alSourcei(alsrc, AL_LOOPING, loop);
	    alSourcei(alsrc, AL_BUFFER, albuf);

	    alSourcePlay(alsrc);
    }
}

void destroy_sound(int soundid)
{
    Sound *sound = get_sound(soundid);

    stop_sound(soundid);

    release_source(sound->source);
    mempool_release(&sound_pool, sound);
}

void pause_sound(int soundid)
{
	 Sound* sound = get_sound(soundid);

	 if (sound->source)
	 {
		 ALuint alsrc = sound->source->source;
		 alSourcePause(alsrc);
	 }
}

void stop_sound(int soundid)
{
	Sound* sound = get_sound(soundid);
	if (sound->source)
	{
		ALuint alsrc = sound->source->source;
		alSourceStop(alsrc);
		release_source(sound->source);
		sound->source = NULL;
	}
}

void sound_loop(int soundid, bool loop)
{
    Sound *sound = get_sound(soundid);
    sound->loop = loop;

    if(is_sound_playing(sound->id))
    {
        ALuint alsrc = sound->source->source;
	    ALint loop = sound->loop ? AL_TRUE : AL_FALSE;
	    alSourcei(alsrc, AL_LOOPING, loop);
    }
}

void sound_pitch(int soundid, float value)
{
    Sound *sound = get_sound(soundid);
    sound->pitch = value;

    if(is_sound_playing(sound->id))
    {
        ALuint alsrc = sound->source->source;
	    alSourcef(alsrc, AL_PITCH, sound->pitch);
    }
}

void sound_gain(int soundid, float value)
{
    Sound *sound = get_sound(soundid);
    sound->gain = value;

    if(is_sound_playing(sound->id))
    {
        ALuint alsrc = sound->source->source;
	    alSourcef(alsrc, AL_GAIN, sound->gain);
    }
}

float get_sound_pitch(int soundid)
{
    Sound *sound = get_sound(soundid);
    return sound->pitch;
}

float get_sound_gain(int soundid)
{
    Sound *sound = get_sound(soundid);
    return sound->gain;
}

bool get_sound_loop(int soundid)
{
    Sound *sound = get_sound(soundid);
    return sound->loop;
}

bool is_sound_playing(int soundid)
{
    Sound *sound = get_sound(soundid);

    if(!sound->source)
        return false;

    ALuint src = sound->source->source;
    ALint state;

	alGetSourcei(src, AL_SOURCE_STATE, &state);

    if(state == AL_PLAYING)
        return true;

    return false;
}

static Source *take_source(void)
{
    for(size_t i = 0; i < source_count; ++i)
    {
        Source *source = &sources[i];

        if(!source->used)
        {
            source->used = true;
            return source;
        }
    }

    return NULL;
}

static void release_source(Source *source)
{
    if(!source)
        return;

    source->used = false;
}

static Buffer *get_sound_buffer(int bufferid)
{
    return mempool_get_addr(&buffer_pool, bufferid);
}

static Sound *get_sound(int sound)
{
    return mempool_get_addr(&sound_pool, sound);
}

