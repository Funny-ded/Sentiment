import tweepy
import json
from datetime import datetime, timedelta, timezone


class Twitter:
    CONFIG_FILENAME = "system\\config.json"  # config file for using api

    TWEET_FIELDS = 'id,text,created_at,in_reply_to_user_id'
    TWEET_EXCLUDE = 'retweets,replies'
    TWEET_MAX_RESULTS = 100

    def __init__(self):
        with open(self.CONFIG_FILENAME, encoding='utf-8') as read_file:
            config = json.load(read_file)

        self.client = tweepy.Client(bearer_token=config["bearer_token"])
        self.users = config["users"]

        self.start_time = config["start_date"]

        # https://developer.twitter.com/en/docs/twitter-ads-api/timezones
        self.end_time = datetime.now().strftime("%Y-%m-%dT%H:%M:%SZ") # "%Y-%m-%dT%H:%M:%SZ"
        del config

    def pull_data(self):
        tw = []
        for i in range(len(self.users) // 100):
            left = 100 * i
            right = 100 * (i + 1) if 100 * (i + 1) < len(self.users) else len(self.users)

            users = self.client.get_users(usernames=','.join(self.users[left:right])).data
            tw += self.process_user_list(users)
        return tw

    def pull_new_data(self):
        tw = self.pull_data()
        print(f'Num of received messages: {str(len(tw))}')
        return tw

    # todo: try to use paginator to iterate over pages
    def process_user_list(self, users: list):
        tw = list()
        for user in users:
            user_data_num = 0
            first_response = True

            while first_response or response.meta["result_count"] > 0:
                if first_response:
                    first_response = False
                    response = self.get_tweets_by_user(user)
                elif "next_token" in response.meta.keys():
                    response = self.get_tweets_by_user(user, response.meta["next_token"])
                else:  # "next_token" does not exists
                    break

                if response.data is None:
                    break
                for tweet in response.data:
                    if not tweet.in_reply_to_user_id: # exclude replies
                        tw.append(self.tweet_to_dict(tweet))
                        user_data_num += 1

            print(f'Messages of {str(user)} received. Num of collected data: {str(user_data_num)}')

        return tw

    # https://docs.tweepy.org/en/stable/client.html#tweepy.Client.get_users_tweets
    # read the description of pagination token
    def get_tweets_by_user(self, user, pagination_token=None):
        return self.client.get_users_tweets(
            id=user.id,
            exclude=self.TWEET_EXCLUDE,
            start_time=self.start_time,
            end_time=self.end_time,
            tweet_fields=self.TWEET_FIELDS,
            max_results=self.TWEET_MAX_RESULTS,
            pagination_token=pagination_token)

    @staticmethod
    def tweet_to_dict(tweet) -> dict:
        return {
            "id": tweet.id,
            "text": tweet.text,
            "filtered_text": "",
            "words": [],
            "timestamp": int(datetime(
                year=tweet.created_at.year,
                month=tweet.created_at.month,
                day=tweet.created_at.day,
                hour=tweet.created_at.hour,
                minute=tweet.created_at.minute,
                second=tweet.created_at.second).replace(tzinfo=timezone.utc).timestamp()),
            "binary": 0,
            "ternary": 0
                }
