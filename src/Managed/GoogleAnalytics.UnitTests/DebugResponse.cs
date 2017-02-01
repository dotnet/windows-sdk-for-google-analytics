using System.Collections.Generic;
using System.Runtime.Serialization;

namespace GoogleAnalytics.UnitTests
{
    [DataContract]
    public class DebugResponse
    {
        [DataMember(Name = "hitParsingResult")]
        public IList<HitParsingResult> HitParsingResult { get; private set; }

        [DataMember(Name = "parserMessage")]
        public IList<ParserMessage> ParserMessage { get; private set; }
    }

    [DataContract]
    public class ParserMessage
    {
        [DataMember(Name = "messageType")]
        public string MessageType { get; private set; }

        [DataMember(Name = "description")]
        public string Description { get; private set; }

        [DataMember(Name = "messageCode")]
        public string MessageCode { get; private set; }

        [DataMember(Name = "parameter")]
        public string Parameter { get; private set; }
    }

    [DataContract]
    public class HitParsingResult
    {
        [DataMember(Name = "valid")]
        public bool Valid { get; private set; }

        [DataMember(Name = "parserMessage")]
        public IList<ParserMessage> ParserMessage { get; private set; }

        [DataMember(Name = "hit")]
        public string Hit { get; private set; }
    }
}
