
# Deploy Stack

``` bash
aws cloudformation deploy --template-file iot-thing.yml --stack-name WeatherStation --region us-east-1  --capabilities "CAPABILITY_NAMED_IAM" "CAPABILITY_AUTO_EXPAND" "CAPABILITY_IAM" --no-fail-on-empty-changeset
```