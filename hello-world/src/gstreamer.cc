#include <gst/gst.h>
#include <napi.h>

using namespace Napi; 

Number Play (const CallbackInfo& info)
{
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *message;

  gst_init(NULL, NULL);

  pipeline = gst_parse_launch(
    "playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm"
  , NULL);

  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  bus = gst_element_get_bus(pipeline);
  message = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

  if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR)
  {
    g_error("An error occured");
  }

  gst_message_unref(message);
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return Number::New(info.Env(), 0);
}

Object Init (Env env, Object exports)
{
  exports.Set("play", Function::New(env, Play));

  return exports;
}

NODE_API_MODULE(addon, Init);
