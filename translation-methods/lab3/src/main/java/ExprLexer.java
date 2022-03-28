// Generated from Expr.g4 by ANTLR 4.9.2

import java.util.HashMap;
import java.util.Map;

import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class ExprLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		WS=1, PLUS=2, MINUS=3, ASTER=4, SLASH=5, SLASH2=6, LPAREN=7, RPAREN=8, 
		EQ=9, SEMICOLON=10, VAR=11, NUM=12;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"WS", "PLUS", "MINUS", "ASTER", "SLASH", "SLASH2", "LPAREN", "RPAREN", 
			"EQ", "SEMICOLON", "VAR", "NUM"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "WS", "PLUS", "MINUS", "ASTER", "SLASH", "SLASH2", "LPAREN", "RPAREN", 
			"EQ", "SEMICOLON", "VAR", "NUM"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public ExprLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "Expr.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\16u\b\1\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\3\2\6\2\35\n\2\r\2\16\2\36\3\2\3\2\3\3\7\3$\n\3\f"+
		"\3\16\3\'\13\3\3\3\3\3\3\4\7\4,\n\4\f\4\16\4/\13\4\3\4\3\4\3\5\7\5\64"+
		"\n\5\f\5\16\5\67\13\5\3\5\3\5\3\6\7\6<\n\6\f\6\16\6?\13\6\3\6\3\6\3\7"+
		"\7\7D\n\7\f\7\16\7G\13\7\3\7\3\7\3\7\3\b\7\bM\n\b\f\b\16\bP\13\b\3\b\3"+
		"\b\3\t\7\tU\n\t\f\t\16\tX\13\t\3\t\3\t\3\n\7\n]\n\n\f\n\16\n`\13\n\3\n"+
		"\3\n\3\13\7\13e\n\13\f\13\16\13h\13\13\3\13\3\13\3\f\6\fm\n\f\r\f\16\f"+
		"n\3\r\6\rr\n\r\r\r\16\rs\2\2\16\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23"+
		"\13\25\f\27\r\31\16\3\2\5\5\2\13\f\17\17\"\"\4\2C\\c|\3\2\62;\2\u0080"+
		"\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2"+
		"\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2"+
		"\2\31\3\2\2\2\3\34\3\2\2\2\5%\3\2\2\2\7-\3\2\2\2\t\65\3\2\2\2\13=\3\2"+
		"\2\2\rE\3\2\2\2\17N\3\2\2\2\21V\3\2\2\2\23^\3\2\2\2\25f\3\2\2\2\27l\3"+
		"\2\2\2\31q\3\2\2\2\33\35\t\2\2\2\34\33\3\2\2\2\35\36\3\2\2\2\36\34\3\2"+
		"\2\2\36\37\3\2\2\2\37 \3\2\2\2 !\b\2\2\2!\4\3\2\2\2\"$\5\3\2\2#\"\3\2"+
		"\2\2$\'\3\2\2\2%#\3\2\2\2%&\3\2\2\2&(\3\2\2\2\'%\3\2\2\2()\7-\2\2)\6\3"+
		"\2\2\2*,\5\3\2\2+*\3\2\2\2,/\3\2\2\2-+\3\2\2\2-.\3\2\2\2.\60\3\2\2\2/"+
		"-\3\2\2\2\60\61\7/\2\2\61\b\3\2\2\2\62\64\5\3\2\2\63\62\3\2\2\2\64\67"+
		"\3\2\2\2\65\63\3\2\2\2\65\66\3\2\2\2\668\3\2\2\2\67\65\3\2\2\289\7,\2"+
		"\29\n\3\2\2\2:<\5\3\2\2;:\3\2\2\2<?\3\2\2\2=;\3\2\2\2=>\3\2\2\2>@\3\2"+
		"\2\2?=\3\2\2\2@A\7\61\2\2A\f\3\2\2\2BD\5\3\2\2CB\3\2\2\2DG\3\2\2\2EC\3"+
		"\2\2\2EF\3\2\2\2FH\3\2\2\2GE\3\2\2\2HI\7\61\2\2IJ\7\61\2\2J\16\3\2\2\2"+
		"KM\5\3\2\2LK\3\2\2\2MP\3\2\2\2NL\3\2\2\2NO\3\2\2\2OQ\3\2\2\2PN\3\2\2\2"+
		"QR\7*\2\2R\20\3\2\2\2SU\5\3\2\2TS\3\2\2\2UX\3\2\2\2VT\3\2\2\2VW\3\2\2"+
		"\2WY\3\2\2\2XV\3\2\2\2YZ\7+\2\2Z\22\3\2\2\2[]\5\3\2\2\\[\3\2\2\2]`\3\2"+
		"\2\2^\\\3\2\2\2^_\3\2\2\2_a\3\2\2\2`^\3\2\2\2ab\7?\2\2b\24\3\2\2\2ce\5"+
		"\3\2\2dc\3\2\2\2eh\3\2\2\2fd\3\2\2\2fg\3\2\2\2gi\3\2\2\2hf\3\2\2\2ij\7"+
		"=\2\2j\26\3\2\2\2km\t\3\2\2lk\3\2\2\2mn\3\2\2\2nl\3\2\2\2no\3\2\2\2o\30"+
		"\3\2\2\2pr\t\4\2\2qp\3\2\2\2rs\3\2\2\2sq\3\2\2\2st\3\2\2\2t\32\3\2\2\2"+
		"\17\2\36%-\65=ENV^fns\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}