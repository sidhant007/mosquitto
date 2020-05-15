#include <cstring>

#include <mosquittopp.h>

static int run = -1;
static int sent_mid = -1;

class mosquittopp_test : public mosqpp::mosquittopp
{
	public:
		mosquittopp_test(const char *id);

		void on_connect(int rc);
		void on_publish(int mid);
};

mosquittopp_test::mosquittopp_test(const char *id) : mosqpp::mosquittopp(id)
{
}

void mosquittopp_test::on_connect(int rc)
{
	if(rc){
		exit(1);
	}else{
    const char* messages[] = {"message1", "message2", "message3"};
    int lengths[] = {strlen("message1"), strlen("message2"), strlen("message3")};
		publishm(&sent_mid, "pub/qos0/test", messages, lengths, 3, 0, false);
	}
}

void mosquittopp_test::on_publish(int mid)
{
	if(sent_mid == mid){
		disconnect();
	}else{
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	struct mosquittopp_test *mosq;

	int port = atoi(argv[1]);

	mosqpp::lib_init();

	mosq = new mosquittopp_test("publish-qos0-test");

	mosq->connect("localhost", port, 60);

	while(run == -1){
		mosq->loop();
	}

	mosqpp::lib_cleanup();

	return run;
}
